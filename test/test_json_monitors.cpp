#include "catch.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "reelay/json.hpp"
#include "reelay/monitors.hpp"

using time_t = int64_t;
using input_t = reelay::json;

TEST_CASE("Discrete Timed Update1") {

  SECTION("Untimed") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", true}});
    sequence.push_back(input_t{{"p2", false}});
    sequence.push_back(input_t({}));
    sequence.push_back(input_t({}));
    sequence.push_back(input_t{{"p1", false}});
    sequence.push_back(input_t{{"p1", true}});
    sequence.push_back(input_t{{"p2", true}});
    sequence.push_back(input_t{{"p2", false}});
    sequence.push_back(input_t({}));
    sequence.push_back(input_t{{"p1", false}});
    sequence.push_back(input_t({}));
    sequence.push_back(input_t{{"p1", true}, {"p2", true}});
    sequence.push_back(input_t{{"p2", false}});

    auto monitor
        = reelay::discrete_timed<time_t>::json_monitor("{p1} since {p2}");

    auto results = std::vector<input_t>();

    for (const auto &s : sequence) {
      auto r = monitor.update(s);
      results.push_back(r);
    }

    auto expected = std::vector<input_t>();
    expected.push_back(input_t{{"value", false}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", false}});
    expected.push_back(input_t{{"value", false}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", false}});
    expected.push_back(input_t{{"value", false}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});

    CHECK(results == expected);
    CHECK(monitor.now() == sequence.size() - 1) ;
  }

  SECTION("Discrete Timed") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", true}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", true}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", false}, {"p2", false}});

    auto monitor
        = reelay::discrete_timed<time_t>::json_monitor("{p1} since[2:] {p2}");

    auto results = std::vector<input_t>();

    for (const auto &s : sequence) {
      auto r = monitor.update(s);
      results.push_back(r);
    }

    auto expected = std::vector<input_t>();
    expected.push_back(input_t{{"value", false}});
    expected.push_back(input_t{{"value", false}});
    expected.push_back(input_t{{"value", false}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", false}});

    CHECK(results == expected);
    CHECK(monitor.now() == sequence.size() - 1);
  }

  SECTION("Untimed Data") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"open", "1", "read"});
    sequence.push_back(input_t{"open", "2", "read"});
    sequence.push_back(input_t{"open", "3", "read"});
    sequence.push_back(input_t{"open", "4", "read"});
    sequence.push_back(input_t{"close", "4"});
    sequence.push_back(input_t{"open", "3", "write"});
    sequence.push_back(input_t{"open", "4", "write"});
    sequence.push_back(input_t{"close", "4"});
    sequence.push_back(input_t{"close", "3"});
    sequence.push_back(input_t{"close", "2"});
    sequence.push_back(input_t{"close", "1"});
    sequence.push_back(input_t{"close", "4"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto monitor = reelay::discrete_timed<time_t>::json_monitor(
        "forall[file]. {$0: close, $1: *file} implies (exists[mode]. "
        "pre(!{$0: close, $1: *file} since {$0: open, $1: *file, $2: "
        "*mode}))",
        extra_args);

    auto result = std::vector<input_t>();

    for (const auto &row : sequence) {
      auto r = monitor.update(row);
      result.push_back(r);
    }

    auto expected = std::vector<input_t>();
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", false}});

    CHECK(result == expected);
    CHECK(monitor.now() == sequence.size() - 1);
  }

  SECTION("Discrete Timed Data") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"sensor_id", "1"}, {"action", "calibrated"}});
    sequence.push_back(input_t{{"sensor_id", "2"}, {"action", "calibrated"}});
    sequence.push_back(input_t{{"sensor_id", "1"}, {"action", "send_data"}});
    sequence.push_back(input_t{{"sensor_id", "1"}, {"action", "send_data"}});
    sequence.push_back(input_t{{"sensor_id", "1"}, {"action", "send_data"}});
    sequence.push_back(input_t{{"sensor_id", "1"}, {"action", "calibrated"}});
    sequence.push_back(input_t{{"sensor_id", "2"}, {"action", "send_data"}});
    sequence.push_back(input_t{{"sensor_id", "1"}, {"action", "send_data"}});
    sequence.push_back(input_t{{"sensor_id", "1"}, {"action", "send_data"}});
    sequence.push_back(input_t{{"sensor_id", "2"}, {"action", "send_data"}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto monitor = reelay::discrete_timed<time_t>::json_monitor(
        "forall[sensor]."
        "{sensor_id: *sensor, action: send_data}"
        "implies"
        "once[:4]{sensor_id: *sensor, action : calibrated}",
        extra_args);

    auto result = std::vector<input_t>();

    for (const auto &row : sequence) {
      auto r = monitor.update(row);
      result.push_back(r);
    }

    auto expected = std::vector<input_t>();
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", false}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", true}});
    expected.push_back(input_t{{"value", false}});

    CHECK(result == expected);
    CHECK(monitor.now() == sequence.size() - 1);
  }
}

TEST_CASE("Discrete Timed Update") {
  SECTION("Untimed") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", true}});
    sequence.push_back(input_t{{"p2", false}});
    sequence.push_back(input_t({}));
    sequence.push_back(input_t({}));
    sequence.push_back(input_t{{"p1", false}});
    sequence.push_back(input_t{{"p1", true}});
    sequence.push_back(input_t{{"p2", true}});
    sequence.push_back(input_t{{"p2", false}});
    sequence.push_back(input_t({}));
    sequence.push_back(input_t{{"p1", false}});
    sequence.push_back(input_t({}));
    sequence.push_back(input_t{{"p1", true}, {"p2", true}});
    sequence.push_back(input_t{{"p2", false}});

    auto monitor = reelay::condensing<time_t>::json_monitor("{p1} since {p2}");

    auto results = std::vector<input_t>();

    for (const auto &s : sequence) {
      auto r = monitor.update(s);
      results.push_back(r);
    }

    auto expected = std::vector<input_t>();
    expected.push_back(input_t{{"time", 0}, {"value", false}});
    expected.push_back(input_t{{"time", 1}, {"value", true}});
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t{{"time", 5}, {"value", false}});
    expected.push_back(input_t({}));
    expected.push_back(input_t{{"time", 7}, {"value", true}});
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t{{"time", 10}, {"value", false}});
    expected.push_back(input_t({}));
    expected.push_back(input_t{{"time", 12}, {"value", true}});
    expected.push_back(input_t({}));

    CHECK(results == expected);
    CHECK(monitor.now() == sequence.size() - 1);
  }

  SECTION("Discrete Timed") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", true}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", true}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", false}, {"p2", false}});

    auto monitor = reelay::condensing<time_t>::json_monitor(
        "{p1} since[2:] {p2}");

    auto results = std::vector<input_t>();

    for (const auto &s : sequence) {
      auto r = monitor.update(s);
      results.push_back(r);
    }

    auto expected = std::vector<input_t>();
    expected.push_back(input_t{{"time", 0}, {"value", false}});
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t{{"time", 3}, {"value", true}});
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t{{"time", 10}, {"value", false}});

    CHECK(results == expected);
    CHECK(monitor.now() == sequence.size() - 1);
  }

  SECTION("Untimed Data") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"open", "1", "read"});
    sequence.push_back(input_t{"open", "2", "read"});
    sequence.push_back(input_t{"open", "3", "read"});
    sequence.push_back(input_t{"open", "4", "read"});
    sequence.push_back(input_t{"close", "4"});
    sequence.push_back(input_t{"open", "3", "write"});
    sequence.push_back(input_t{"open", "4", "write"});
    sequence.push_back(input_t{"close", "4"});
    sequence.push_back(input_t{"close", "3"});
    sequence.push_back(input_t{"close", "2"});
    sequence.push_back(input_t{"close", "1"});
    sequence.push_back(input_t{"close", "4"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto monitor = reelay::condensing<time_t>::json_monitor(
        "forall[file]. {$0: close, $1: *file} implies (exists[mode]. "
        "pre(!{$0: close, $1: *file} since {$0: open, $1: *file, $2: "
        "*mode}))",
        extra_args);

    auto result = std::vector<input_t>();

    for (const auto &row : sequence) {
      auto r = monitor.update(row);
      result.push_back(r);
    }

    auto expected = std::vector<input_t>();
    expected.push_back(input_t{{"time", 0}, {"value", true}});
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t{{"time", 11}, {"value", false}});

    CHECK(result == expected);
    CHECK(monitor.now() == sequence.size() - 1);
  }

  SECTION("Discrete Timed Data") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"sensor_id", "1"}, {"action", "calibrated"}});
    sequence.push_back(input_t{{"sensor_id", "2"}, {"action", "calibrated"}});
    sequence.push_back(input_t{{"sensor_id", "1"}, {"action", "send_data"}});
    sequence.push_back(input_t{{"sensor_id", "1"}, {"action", "send_data"}});
    sequence.push_back(input_t{{"sensor_id", "1"}, {"action", "send_data"}});
    sequence.push_back(input_t{{"sensor_id", "1"}, {"action", "calibrated"}});
    sequence.push_back(input_t{{"sensor_id", "2"}, {"action", "send_data"}});
    sequence.push_back(input_t{{"sensor_id", "1"}, {"action", "send_data"}});
    sequence.push_back(input_t{{"sensor_id", "1"}, {"action", "send_data"}});
    sequence.push_back(input_t{{"sensor_id", "2"}, {"action", "send_data"}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto monitor = reelay::condensing<time_t>::json_monitor(
        "forall[sensor]."
        "{sensor_id: *sensor, action: send_data}"
        "implies"
        "once[:4]{sensor_id: *sensor, action : calibrated}",
        extra_args);

    auto result = std::vector<input_t>();

    for (const auto &row : sequence) {
      auto r = monitor.update(row);
      result.push_back(r);
    }

    auto expected = std::vector<input_t>();
    expected.push_back(input_t{{"time", 0}, {"value", true}});
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t{{"time", 6}, {"value", false}});
    expected.push_back(input_t{{"time", 7}, {"value", true}});
    expected.push_back(input_t({}));
    expected.push_back(input_t{{"time", 9}, {"value", false}});

    CHECK(result == expected);
    CHECK(monitor.now() == sequence.size() - 1);
  }
}

TEST_CASE("Dense Timed") {

  SECTION("Since") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"time", 10}, {"p1", false}, {"p2", true}});
    sequence.push_back(input_t{{"time", 20}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"time", 30}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"time", 60}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"time", 70}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"time", 90}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"time", 115}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_t{{"time", 156}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"time", 175}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"time", 201}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"time", 248}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"time", 287}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"time", 300}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"time", 315}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"time", 444}});

    auto monitor = reelay::dense_timed<time_t>::json_monitor("{p1} since {p2}");

    auto result = std::vector<input_t>();

    for (const auto &row : sequence) {
      auto r = monitor.update(row);
      if (not r.empty()) {
        result.insert(result.end(), r.begin(), r.end());
      }
    }

    auto expected = std::vector<input_t>();
    expected.push_back(input_t{{"time", 0}, {"value", false}});
    expected.push_back(input_t{{"time", 115}, {"value", true}});
    expected.push_back(input_t{{"time", 201}, {"value", false}});

    CHECK(result == expected);
    CHECK(monitor.now() == 444);
  }

  SECTION("Timed Since") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"time", 10}, {"p1", false}, {"p2", true}});
    sequence.push_back(input_t{{"time", 20}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"time", 30}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"time", 60}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"time", 70}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"time", 90}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"time", 115}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_t{{"time", 156}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"time", 175}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"time", 201}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"time", 248}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"time", 287}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"time", 300}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"time", 315}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"time", 444}});

    auto monitor
        = reelay::dense_timed<time_t>::json_monitor("{p1} since[18:]{p2}");

    auto result = std::vector<input_t>();

    for (const auto &row : sequence) {
      auto r = monitor.update(row);
      if (not r.empty()) {
        result.insert(result.end(), r.begin(), r.end());
      }
    }

    auto expected = std::vector<input_t>();
    expected.push_back(input_t{{"time", 0}, {"value", false}});
    expected.push_back(input_t{{"time", 133}, {"value", true}});
    expected.push_back(input_t{{"time", 201}, {"value", false}});

    CHECK(result == expected);
    CHECK(monitor.now() == 444);
  }

  SECTION("Avec Interpolation Lineaire") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0.0}, {"speed", 3.0}});
    sequence.push_back(input_t{{"time", 10.0}, {"speed", 9.0}});
    sequence.push_back(input_t{{"time", 12.0}, {"speed", 13.0}});
    sequence.push_back(input_t{{"time", 14.0}, {"speed", 11.0}});
    sequence.push_back(input_t{{"time", 15.0}, {"speed", 9.0}});
    sequence.push_back(input_t{{"time", 18.0}, {"speed", 6.0}});
    sequence.push_back(input_t{{"time", 19.0}, {"speed", 14.0}});

    auto monitor
        = reelay::dense_timed<double, reelay::piecewise::LINEAR>::json_monitor(
            "{speed > 12.0}");

    auto result = std::vector<input_t>();

    for (const auto &row : sequence) {
      auto r = monitor.update(row);
      if (not r.empty()) {
        result.insert(result.end(), r.begin(), r.end());
      }
    }

    auto expected = std::vector<input_t>();
    expected.push_back(input_t{{"time", 0}, {"value", false}});
    expected.push_back(input_t{{"time", 11.5}, {"value", true}});
    expected.push_back(input_t{{"time", 13.0}, {"value", false}});
    expected.push_back(input_t{{"time", 18.75}, {"value", true}});

    CHECK(result == expected);
    CHECK(monitor.now() == 19.0);
  }

  SECTION("Timed Since") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(
        input_t{{"time", 0}});
    sequence.push_back(
        input_t{{"time", 6}, {"sensor_id", "1"}, {"action", "calibrated"}});
    sequence.push_back(
        input_t{{"time", 12}, {"sensor_id", "2"}, {"action", "calibrated"}});
    sequence.push_back(
        input_t{{"time", 15}, {"sensor_id", "1"}, {"action", "send_data"}});
    sequence.push_back(
        input_t{{"time", 24}, {"sensor_id", "1"}, {"action", "send_data"}});
    sequence.push_back(
        input_t{{"time", 27}, {"sensor_id", "1"}, {"action", "send_data"}});
    sequence.push_back(
        input_t{{"time", 32}, {"sensor_id", "1"}, {"action", "calibrated"}});
    sequence.push_back(
        input_t{{"time", 46}, {"sensor_id", "2"}, {"action", "send_data"}});
    sequence.push_back(
        input_t{{"time", 49}, {"sensor_id", "1"}, {"action", "send_data"}});
    sequence.push_back(
        input_t{{"time", 56}, {"sensor_id", "1"}, {"action", "send_data"}});
    sequence.push_back(
        input_t{{"time", 62}, {"sensor_id", "2"}, {"action", "send_data"}});
    sequence.push_back(
        input_t{{"time", 67}, {"sensor_id", "1"}, {"action", "send_data"}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto monitor = reelay::dense_timed<time_t>::json_monitor(
        "forall[sensor]."
        "{sensor_id: *sensor, action: send_data}"
        "implies"
        "once[:40]{sensor_id: *sensor, action : calibrated}",
        extra_args);

    auto result = std::vector<input_t>();

    for (const auto &row : sequence) {
      auto r = monitor.update(row);
      if (not r.empty()) {
        result.insert(result.end(), r.begin(), r.end());
      }
    }

    auto expected = std::vector<input_t>();
    expected.push_back(input_t{{"time", 0}, {"value", true}});
    expected.push_back(input_t{{"time", 62}, {"value", false}});

    CHECK(result == expected);
    CHECK(monitor.now() == 67);
  }


}
TEST_CASE("Discrete Timed Robustness") {
  using output_t = double;

  SECTION("Always") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 8}, {"p2", -1}});
    sequence.push_back(input_t{{"p1", 7}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", 5}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 6}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 16}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 61}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 3.0}, {"p2", 0}});

    auto monitor
        = reelay::discrete_timed<time_t>::robustness<output_t>::json_monitor(
            "historically{p1}");

    auto result = std::vector<input_t>();

    for (const auto &s : sequence) {
      auto r = monitor.update(s);
      result.push_back(r);
    }

    auto expected = std::vector<input_t>();
    expected.push_back(input_t{{"value", 8.0}});
    expected.push_back(input_t{{"value", 7.0}});
    expected.push_back(input_t{{"value", 5.0}});
    expected.push_back(input_t{{"value", 5.0}});
    expected.push_back(input_t{{"value", 5.0}});
    expected.push_back(input_t{{"value", 5.0}});
    expected.push_back(input_t{{"value", 3.0}});

    CHECK(result == expected);
    CHECK(monitor.now() == sequence.size() - 1);
  }

  SECTION("Always") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 8}, {"p2", -1}});
    sequence.push_back(input_t{{"p1", 7}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", 5}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 6}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 16}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 61}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 3.0}, {"p2", 0}});

    auto monitor
        = reelay::discrete_timed<time_t>::robustness<output_t>::json_monitor(
            "historically[:2]{p1}");

    auto result = std::vector<input_t>();

    for (const auto &s : sequence) {
      auto r = monitor.update(s);
      result.push_back(r);
    }

    auto expected = std::vector<input_t>();
    expected.push_back(input_t{{"value", 8.0}});
    expected.push_back(input_t{{"value", 7.0}});
    expected.push_back(input_t{{"value", 5.0}});
    expected.push_back(input_t{{"value", 5.0}});
    expected.push_back(input_t{{"value", 5.0}});
    expected.push_back(input_t{{"value", 6.0}});
    expected.push_back(input_t{{"value", 3.0}});

    CHECK(result == expected);
    CHECK(monitor.now() == sequence.size() - 1);
  }

  SECTION("Always") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 8}, {"p2", -1}});
    sequence.push_back(input_t{{"p1", 7}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", 5}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 6}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 16}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 61}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 3.0}, {"p2", 0}});

    auto monitor
        = reelay::condensing<time_t>::robustness<output_t>::json_monitor(
            "historically{p1}");

    auto result = std::vector<input_t>();

    for (const auto &s : sequence) {
      auto r = monitor.update(s);
      result.push_back(r);
    }

    auto expected = std::vector<input_t>();
    expected.push_back(input_t{{"time", 0}, {"value", 8.0}});
    expected.push_back(input_t{{"time", 1}, {"value", 7.0}});
    expected.push_back(input_t{{"time", 2}, {"value", 5.0}});
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t{{"time", 6}, {"value", 3.0}});

    CHECK(result == expected);
    CHECK(monitor.now() == sequence.size() - 1);
  }

  SECTION("Always") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 8}, {"p2", -1}});
    sequence.push_back(input_t{{"p1", 7}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", 5}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 6}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 16}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 61}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 3.0}, {"p2", 0}});

    auto monitor
        = reelay::condensing<time_t>::robustness<output_t>::json_monitor(
            "historically[:2]{p1}");

    auto result = std::vector<input_t>();

    for (const auto &s : sequence) {
      auto r = monitor.update(s);
      result.push_back(r);
    }

    auto expected = std::vector<input_t>();
    expected.push_back(input_t{{"time", 0}, {"value", 8.0}});
    expected.push_back(input_t{{"time", 1}, {"value", 7.0}});
    expected.push_back(input_t{{"time", 2}, {"value", 5.0}});
    expected.push_back(input_t({}));
    expected.push_back(input_t({}));
    expected.push_back(input_t{{"time", 5}, {"value", 6.0}});
    expected.push_back(input_t{{"time", 6}, {"value", 3.0}});

    CHECK(result == expected);
    CHECK(monitor.now() == sequence.size() - 1);
  }

  SECTION("Timed Once") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x", 11}});
    sequence.push_back(input_t{{"time", 10}, {"x", 9}});
    sequence.push_back(input_t{{"time", 20}, {"x", 13}});
    sequence.push_back(input_t{{"time", 30}, {"x", 17}});
    sequence.push_back(input_t{{"time", 60}, {"x", 15}});
    sequence.push_back(input_t{{"time", 70}, {"x", 4}});
    sequence.push_back(input_t{{"time", 100}});

    auto monitor
        = reelay::dense_timed<time_t>::robustness<output_t>::json_monitor(
            "once[:24]{x}");

    auto result = std::vector<input_t>();

    for (const auto &row : sequence) {
      auto r = monitor.update(row);
      if (not r.empty()) {
        result.insert(result.end(), r.begin(), r.end());
      }
    }

    auto expected = std::vector<input_t>();
    expected.push_back(input_t{{"time", 0}, {"value", 11.0}});
    expected.push_back(input_t{{"time", 20}, {"value", 13.0}});
    expected.push_back(input_t{{"time", 30}, {"value", 17.0}});
    expected.push_back(input_t{{"time", 84}, {"value", 15.0}});
    expected.push_back(input_t{{"time", 94}, {"value", 4.0}});

    CHECK(result == expected);
    CHECK(monitor.now() == 100);
  }
}
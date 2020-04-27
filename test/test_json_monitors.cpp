#include "catch.hpp"
#include "iostream"
#include "reelay/json.hpp"
#include "reelay/monitors.hpp"
#include "string"
#include "vector"

using input_t = reelay::json;

TEST_CASE("Discrete Timed") {
  SECTION("Since") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", true}});
    sequence.push_back(input_t{{"p2", false}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"p1", false}});
    sequence.push_back(input_t{{"p1", true}});
    sequence.push_back(input_t{{"p2", true}});
    sequence.push_back(input_t{{"p2", false}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"p1", false}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"p1", true}, {"p2", true}});
    sequence.push_back(input_t{{"p2", false}});

    auto monitor =
        reelay::discrete_timed<int64_t>::monitor("{p1} since {p2}");

    auto results = std::vector<input_t>();

    for (const auto &s : sequence) {
      auto r = monitor->update(s);
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
  }

  SECTION("DsPTL") {
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

    auto monitor =
        reelay::discrete_timed<int64_t>::monitor("{p1} since[2:] {p2}");

    auto results = std::vector<input_t>();

    for (const auto &s : sequence) {
      auto r = monitor->update(s);
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
  }

  SECTION("Neither Open Nor Close") {
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

    auto monitor = reelay::discrete_timed<int64_t>::monitor(
        "forall[file]. {$0: close, $1: *file} implies (exists[mode]. "
        "pre(!{$0: close, $1: *file} since {$0: open, $1: *file, $2: "
        "*mode}))",
        extra_args);

    auto result = std::vector<input_t>();

    for (const auto &row : sequence) {
      auto r = monitor->update(row);
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
  }

  SECTION("Timed Since") {
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

    auto monitor = reelay::discrete_timed<int64_t>::monitor(
        "forall[sensor]."
          "{sensor_id: *sensor, action: send_data}" 
            "implies"
              "once[:4]{sensor_id: *sensor, action : calibrated}",
        extra_args);

    auto result = std::vector<input_t>();

    for (const auto &row : sequence) {
      auto r = monitor->update(row);
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
  }
}

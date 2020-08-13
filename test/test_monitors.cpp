/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "catch.hpp"
//
#include "iostream"
#include "reelay/monitors.hpp"
#include "vector"

TEST_CASE("Options") {
  SECTION("Dense Timed Const Boolean") {
    auto opts = reelay::basic_options()
                    .with_interpolation(reelay::piecewise::linear)
                    .with_time_field_name("timestamp")
                    .with_value_field_name("verdict");

    CHECK(opts.get_interpolation() == reelay::piecewise::linear);
    CHECK(opts.get_time_field_name() == "timestamp");
    CHECK(opts.get_value_field_name() == "verdict");
  }
}

TEST_CASE("Dense Timed") {
  using time_type = double;
  using value_type = double;
  using input_t = reelay::json;
  using output_t = reelay::json;

  SECTION("Dense Timed Const Boolean") {
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

    auto opts = reelay::dense_timed<time_type>::monitor<input_t>::options()
                    .with_interpolation(reelay::piecewise::constant)
                    .with_time_field_name("timestamp")
                    .with_value_field_name("verdict");

    auto monitor = reelay::make_monitor("{p1} since[:24] {p2}", opts);

    auto result = std::vector<output_t>();

    for (const auto &s : sequence) {
      auto r = monitor.update(s);
      result.insert(result.end(), r.begin(), r.end());
    }

    auto expected = std::vector<output_t>();
    expected.push_back(output_t{{"timestamp", 0.0}, {"verdict", false}});
    expected.push_back(output_t{{"timestamp", 115.0}, {"verdict", true}});
    expected.push_back(output_t{{"timestamp", 180.0}, {"verdict", false}});

    CHECK(result == expected);

    auto timestamp = output_t{{"timestamp", 444.0}};
    CHECK(monitor.now() == timestamp);
  }

  SECTION("Dense Timed Linear Boolean") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x1", -2}});
    sequence.push_back(input_t{{"time", 1}, {"x1", -2}});
    sequence.push_back(input_t{{"time", 2}, {"x1", -1}});
    sequence.push_back(input_t{{"time", 4}, {"x1", 1}});
    sequence.push_back(input_t{{"time", 5}, {"x1", 2}});
    sequence.push_back(input_t{{"time", 6}, {"x1", 2}});
    sequence.push_back(input_t{{"time", 7}, {"x1", 1}});
    sequence.push_back(input_t{{"time", 9}, {"x1", -1}});
    sequence.push_back(input_t{{"time", 10}, {"x1", -2}});
    sequence.push_back(input_t{{"time", 11}, {"x1", 0}});
    sequence.push_back(input_t{{"time", 12}, {"x1", 0}});
    sequence.push_back(input_t{{"time", 13}, {"x1", 2}});
    sequence.push_back(input_t{{"time", 14}, {"x1", 0}});
    sequence.push_back(input_t{{"time", 15}, {"x1", -2}});

    auto opts = reelay::dense_timed<time_type>::monitor<input_t>::options()
                    .with_interpolation(reelay::piecewise::linear);

    auto monitor = reelay::make_monitor("{x1 < 0}", opts);

    auto result = std::vector<output_t>();

    for (const auto &s : sequence) {
      auto r = monitor.update(s);
      result.insert(result.end(), r.begin(), r.end());
    }

    auto expected = std::vector<output_t>();
    expected.push_back(output_t{{"time", 0.0}, {"value", true}});
    expected.push_back(output_t{{"time", 3.0}, {"value", false}});
    expected.push_back(output_t{{"time", 8.0}, {"value", true}});
    expected.push_back(output_t{{"time", 11.0}, {"value", false}});
    expected.push_back(output_t{{"time", 14.0}, {"value", true}});

    CHECK(result == expected);

    auto timestamp = output_t{{"time", 15.0}};
    CHECK(monitor.now() == timestamp);
  }


  SECTION("Dense Timed Const Data") {
      std::vector<input_t> sequence = std::vector<input_t>();

      sequence.push_back(
          input_t{{"time", 0}, {"state", "arrived"}, {"arg1", "a"}});
      sequence.push_back(
          input_t{{"time", 1}, {"state", "open"}, {"arg1", "a"}});
      sequence.push_back(
          input_t{{"time", 4}, {"state", "open"}, {"arg1", "a"}});
      sequence.push_back(
          input_t{{"time", 6}, {"state", "open"}, {"arg1", "a"}});
      sequence.push_back(input_t{{"time", 7}, {"state", "close"}});
      sequence.push_back(input_t{{"time", 9}});

      auto manager = std::make_shared<reelay::binding_manager>();
      auto opts = reelay::dense_timed<time_type>::monitor<input_t>::options()
                      .with_data_manager(manager);

      auto monitor = reelay::make_monitor(
          "exists[arg].({state: open, arg1: *arg} since {state: arrived, arg1: "
          "*arg})",
          opts);

      auto result = std::vector<output_t>();
      for (const auto &s : sequence) {
        auto r = monitor.update(s);
        result.insert(result.end(), r.begin(), r.end());
      }

      auto expected = std::vector<output_t>();
      expected.push_back(output_t{{"time", 0.0}, {"value", true}});
      expected.push_back(output_t{{"time", 7.0}, {"value", false}});

      CHECK(result == expected);

      auto timestamp = output_t{{"time", 9.0}};
      CHECK(monitor.now() == timestamp);
    }

    SECTION("Dense Timed Const Robustness") {
      std::vector<input_t> sequence = std::vector<input_t>();

      sequence.push_back(input_t{{"time", 0}, {"x", 11}});
      sequence.push_back(input_t{{"time", 10}, {"x", 9}});
      sequence.push_back(input_t{{"time", 20}, {"x", 13}});
      sequence.push_back(input_t{{"time", 30}, {"x", 17}});
      sequence.push_back(input_t{{"time", 60}, {"x", 15}});
      sequence.push_back(input_t{{"time", 70}, {"x", 4}});
      sequence.push_back(input_t{{"time", 90}});

      auto opts = reelay::dense_timed<time_type>::robustness<
          value_type>::monitor<input_t>::options();

      auto monitor = reelay::make_monitor("once[12:24]{x}", opts);

      auto result = std::vector<output_t>();

      for (const auto &s : sequence) {
        auto r = monitor.update(s);
        result.insert(result.end(), r.begin(), r.end());
      }

      auto bot = -reelay::infinity<value_type>::value();
      auto expected = std::vector<output_t>();
      expected.push_back(output_t{{"time", 0.0}, {"value", bot}});
      expected.push_back(output_t{{"time", 12.0}, {"value", 11.0}});
      expected.push_back(output_t{{"time", 32.0}, {"value", 13.0}});
      expected.push_back(output_t{{"time", 42.0}, {"value", 17.0}});
      expected.push_back(output_t{{"time", 84.0}, {"value", 15.0}});

      CHECK(result == expected);

      auto timestamp = output_t{{"time", 90.0}};
      CHECK(monitor.now() == timestamp);
    }
  }

TEST_CASE("Discrete Timed") {
  using time_type = intmax_t;
  using value_type = double;
  using input_t = reelay::json;
  using output_t = reelay::json;

  SECTION("Discrete Timed Condensing") {
    auto sequence = std::vector<input_t>();

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

    auto opts = reelay::discrete_timed<time_type>::monitor<input_t>::options()
                    .enable_condensing();

    auto monitor = reelay::make_monitor("{p1} since[2:4] {p2}", opts);

    auto result = std::vector<output_t>();

    for (const auto &s : sequence) {
      auto r = monitor.update(s);
      if (not r.empty()) {
        result.push_back(r);
      }
    }

    auto expected = std::vector<output_t>();
    expected.push_back(output_t{{"time", 0}, {"value", false}});
    expected.push_back(output_t{{"time", 3}, {"value", true}});
    expected.push_back(output_t{{"time", 6}, {"value", false}});
    expected.push_back(output_t{{"time", 9}, {"value", true}});
    expected.push_back(output_t{{"time", 10}, {"value", false}});

    CHECK(result == expected);

    auto timestamp = output_t{{"time", 10}};
    CHECK(monitor.now() == timestamp);
  }

  SECTION("Discrete Timed NonCondensing") {
    auto sequence = std::vector<input_t>();

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

    auto opts = reelay::discrete_timed<time_type>::monitor<input_t>::options()
                    .disable_condensing();

    auto monitor = reelay::make_monitor("{p1} since[2:4] {p2}", opts);

    auto result = std::vector<output_t>();

    for (const auto &s : sequence) {
      auto r = monitor.update(s);
      if (not r.empty()) {
        result.push_back(r);
      }
    }

    auto expected = std::vector<output_t>();
    expected.push_back(output_t{{"value", false}});
    expected.push_back(output_t{{"value", false}});
    expected.push_back(output_t{{"value", false}});
    expected.push_back(output_t{{"value", true}});
    expected.push_back(output_t{{"value", true}});
    expected.push_back(output_t{{"value", true}});
    expected.push_back(output_t{{"value", false}});
    expected.push_back(output_t{{"value", false}});
    expected.push_back(output_t{{"value", false}});
    expected.push_back(output_t{{"value", true}});
    expected.push_back(output_t{{"value", false}});

    CHECK(result == expected);

    auto timestamp = output_t{{"time", 10}};
    CHECK(monitor.now() == timestamp);
  }

  SECTION("Discrete Timed Data Condensing") {
    auto sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "charlotte"});
    sequence.push_back(input_t{"meet", "bob", "humpty", "dumpty"});

    auto opts = reelay::discrete_timed<time_type>::monitor<input_t>::options()
                    .with_data_manager();

    auto monitor = reelay::make_monitor(
        "exists[file].({$0: access, $1: alice, $2: *file})", opts);

    auto result = std::vector<output_t>();

    for (const auto &s : sequence) {
      auto r = monitor.update(s);
      if (not r.empty()) {
        result.push_back(r);
      }
    }

    auto expected = std::vector<output_t>();
    expected.push_back(output_t{{"time", 0}, {"value", true}});
    expected.push_back(output_t{{"time", 1}, {"value", false}});
    expected.push_back(output_t{{"time", 2}, {"value", true}});
    expected.push_back(output_t{{"time", 3}, {"value", false}});

    CHECK(result == expected);

    auto timestamp = output_t{{"time", 4}};
    CHECK(monitor.now() == timestamp);
  }

  SECTION("Discrete Timed Data NonCondensing") {
    auto sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "charlotte"});
    sequence.push_back(input_t{"meet", "bob", "humpty", "dumpty"});

    auto opts = reelay::discrete_timed<time_type>::monitor<input_t>::options()
                    .with_data_manager()
                    .disable_condensing();

    auto monitor = reelay::make_monitor(
        "exists[file].({$0: access, $1: alice, $2: *file})", opts);

    auto result = std::vector<output_t>();

    for (const auto &s : sequence) {
      auto r = monitor.update(s);
      if (not r.empty()) {
        result.push_back(r);
      }
    }

    auto expected = std::vector<output_t>();
    expected.push_back(output_t{{"value", true}});
    expected.push_back(output_t{{"value", false}});
    expected.push_back(output_t{{"value", true}});
    expected.push_back(output_t{{"value", false}});
    expected.push_back(output_t{{"value", false}});

    CHECK(result == expected);

    auto timestamp = output_t{{"time", 4}};
    CHECK(monitor.now() == timestamp);
  }

  SECTION("Discrete Timed Robustness Condensing") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 3}});
    sequence.push_back(input_t{{"p1", 4}});
    sequence.push_back(input_t{{"p1", 5}});
    sequence.push_back(input_t{{"p1", 1}});
    sequence.push_back(input_t{{"p1", 2}});
    sequence.push_back(input_t{{"p1", -1}});
    sequence.push_back(input_t{{"p1", -2}});
    sequence.push_back(input_t{{"p1", 14}});
    sequence.push_back(input_t{{"p1", 4}});
    sequence.push_back(input_t{{"p1", 4}});
    sequence.push_back(input_t{{"p1", 2}});

    auto opts = reelay::discrete_timed<time_type>::robustness<
                    value_type>::monitor<input_t>::options()
                    .with_time_field_name("timestamp")
                    .with_value_field_name("verdict");

    auto monitor = reelay::make_monitor("historically[:3]{p1>0}", opts);

    auto result = std::vector<output_t>();

    for (const auto &s : sequence) {
      auto r = monitor.update(s);
      if(not r.empty()){
        result.push_back(r);
      }
    }

    auto expected = std::vector<output_t>();
    expected.push_back(output_t{{"timestamp", 0}, {"verdict", 3.0}});
    expected.push_back(output_t{{"timestamp", 3}, {"verdict", 1.0}});
    expected.push_back(output_t{{"timestamp", 5}, {"verdict", -1.0}});
    expected.push_back(output_t{{"timestamp", 6}, {"verdict", -2.0}});
    expected.push_back(output_t{{"timestamp", 10}, {"verdict", 2.0}});

    CHECK(result == expected);

    auto timestamp = output_t{{"timestamp", 10}};
    CHECK(monitor.now() == timestamp);
  }

  SECTION("Discrete Timed Const Robustness NonCondensing") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 3}});
    sequence.push_back(input_t{{"p1", 4}});
    sequence.push_back(input_t{{"p1", 5}});
    sequence.push_back(input_t{{"p1", 1}});
    sequence.push_back(input_t{{"p1", 2}});
    sequence.push_back(input_t{{"p1", -1}});
    sequence.push_back(input_t{{"p1", -2}});
    sequence.push_back(input_t{{"p1", 14}});
    sequence.push_back(input_t{{"p1", 4}});
    sequence.push_back(input_t{{"p1", 4}});
    sequence.push_back(input_t{{"p1", 2}});

    auto opts = reelay::discrete_timed<time_type>::robustness<
                    value_type>::monitor<input_t>::options()
                    .disable_condensing()
                    .with_time_field_name("timestamp")
                    .with_value_field_name("verdict");

    auto monitor = reelay::make_monitor("historically[:3]{p1>0}", opts);

    auto result = std::vector<output_t>();

    for (const auto &s : sequence) {
      auto r = monitor.update(s);
      result.push_back(r);
    }

    auto expected = std::vector<output_t>();
    expected.push_back(output_t{{"verdict", 3.0}});
    expected.push_back(output_t{{"verdict", 3.0}});
    expected.push_back(output_t{{"verdict", 3.0}});
    expected.push_back(output_t{{"verdict", 1.0}});
    expected.push_back(output_t{{"verdict", 1.0}});
    expected.push_back(output_t{{"verdict", -1.0}});
    expected.push_back(output_t{{"verdict", -2.0}});
    expected.push_back(output_t{{"verdict", -2.0}});
    expected.push_back(output_t{{"verdict", -2.0}});
    expected.push_back(output_t{{"verdict", -2.0}});
    expected.push_back(output_t{{"verdict", 2.0}});

    CHECK(result == expected);

    auto timestamp = output_t{{"timestamp", 10}};
    CHECK(monitor.now() == timestamp);
  }
}

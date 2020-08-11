/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "catch.hpp"
#include "iostream"
#include "reelay/common.hpp"
#include "reelay/intervals.hpp"
#include "reelay/json.hpp"
#include "vector"
//
#include "reelay/networks/dense_timed_robustness_0_network.hpp"
#include "reelay/options.hpp"

using input_t = reelay::json;
using value_t = int64_t;
using time_type = int64_t;

using interval = reelay::interval<time_type>;
using robustness_map = reelay::robustness_interval_map<time_type, value_t>;

value_t top = reelay::infinity<value_t>::value();
value_t bot = -reelay::infinity<value_t>::value();

TEST_CASE("Atoms") {
  SECTION("Proposition") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x", 11}});
    sequence.push_back(input_t{{"time", 10}, {"x", 15}});
    sequence.push_back(input_t{{"time", 20}, {"x", 13}});
    sequence.push_back(input_t{{"time", 42}});


    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("{x}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 10), 11));
    expected1.add(std::make_pair(interval::left_open(10, 20), 15));
    expected1.add(std::make_pair(interval::left_open(20, 42), 13));

    CHECK(result1 == expected1);
  }

  SECTION("GreaterEqual") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x1", 3}});
    sequence.push_back(input_t{{"time", 1}, {"x1", 5}});
    sequence.push_back(input_t{{"time", 6}, {"x1", 4}});
    sequence.push_back(input_t{{"time", 9}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("{x1 >= 4}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 1), -1));
    expected1.add(std::make_pair(interval::left_open(1, 6), 1));
    expected1.add(std::make_pair(interval::left_open(6, 9), 0));

    CHECK(result1 == expected1);
  }

  SECTION("GreaterThan") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x1", 3}});
    sequence.push_back(input_t{{"time", 1}, {"x1", 5}});
    sequence.push_back(input_t{{"time", 6}, {"x1", 4}});
    sequence.push_back(input_t{{"time", 9}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("{x1 > 4}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 1), -1));
    expected1.add(std::make_pair(interval::left_open(1, 6), 1));
    expected1.add(std::make_pair(interval::left_open(6, 9), 0));

    CHECK(result1 == expected1);
  }

  SECTION("LessEqual") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x1", 3}});
    sequence.push_back(input_t{{"time", 1}, {"x1", 5}});
    sequence.push_back(input_t{{"time", 6}, {"x1", 4}});
    sequence.push_back(input_t{{"time", 9}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("{x1 <= 4}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 1), 1));
    expected1.add(std::make_pair(interval::left_open(1, 6), -1));
    expected1.add(std::make_pair(interval::left_open(6, 9), 0));

    CHECK(result1 == expected1);
  }

  SECTION("LessThan") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x1", 3}});
    sequence.push_back(input_t{{"time", 1}, {"x1", 5}});
    sequence.push_back(input_t{{"time", 6}, {"x1", 4}});
    sequence.push_back(input_t{{"time", 9}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("{x1 <= 4}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 1), 1));
    expected1.add(std::make_pair(interval::left_open(1, 6), -1));
    expected1.add(std::make_pair(interval::left_open(6, 9), 0));

    CHECK(result1 == expected1);
  }

  SECTION("InBetween_ZeroHoldForward") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x1", 3}});
    sequence.push_back(input_t{{"time", 1}, {"x1", 3}});
    sequence.push_back(input_t{{"time", 3}, {"x1", 6}});
    sequence.push_back(input_t{{"time", 5}, {"x1", 4}});
    sequence.push_back(input_t{{"time", 7}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("{x1 > 3, x1 < 5}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 3), 0));
    expected1.add(std::make_pair(interval::left_open(0, 5), -1));
    expected1.add(std::make_pair(interval::left_open(5, 7), 1));

    CHECK(result1 == expected1);
  }

  SECTION("AtomicTrue") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x", true}});
    sequence.push_back(input_t{{"time", 10}, {"x", false}});
    sequence.push_back(input_t{{"time", 20}, {"x", true}});
    sequence.push_back(input_t{{"time", 42}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("{x: true}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 10), top));
    expected1.add(std::make_pair(interval::left_open(10, 20), bot));
    expected1.add(std::make_pair(interval::left_open(20, 42), top));

    CHECK(result1 == expected1);
  }

  SECTION("AtomicFalse") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x", true}});
    sequence.push_back(input_t{{"time", 10}, {"x", false}});
    sequence.push_back(input_t{{"time", 20}, {"x", true}});
    sequence.push_back(input_t{{"time", 42}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("{x: false}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 10), bot));
    expected1.add(std::make_pair(interval::left_open(10, 20), top));
    expected1.add(std::make_pair(interval::left_open(20, 42), bot));

    CHECK(result1 == expected1);
  }

  SECTION("AtomicString") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x", "hello"}});
    sequence.push_back(input_t{{"time", 10}, {"x", "world"}});
    sequence.push_back(input_t{{"time", 20}, {"x", "alice"}});
    sequence.push_back(input_t{{"time", 42}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("{x: alice}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 20), bot));
    expected1.add(std::make_pair(interval::left_open(20, 42), top));

    CHECK(result1 == expected1);
  }

  SECTION("AtomicNumber") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x", 555}});
    sequence.push_back(input_t{{"time", 10}, {"x", 666}});
    sequence.push_back(input_t{{"time", 20}, {"x", 777}});
    sequence.push_back(input_t{{"time", 42}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("{x: 777}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 20), bot));
    expected1.add(std::make_pair(interval::left_open(20, 42), top));

    CHECK(result1 == expected1);
  }

  SECTION("AtomicAny") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x", 555}});
    sequence.push_back(input_t{{"time", 10}});
    sequence.push_back(input_t{{"time", 20}, {"x", 777}});
    sequence.push_back(input_t{{"time", 42}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("{x: *}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 10), top));
    expected1.add(std::make_pair(interval::left_open(10, 20), bot));
    expected1.add(std::make_pair(interval::left_open(20, 42), top));

    CHECK(result1 == expected1);
  }
}

TEST_CASE("Boolean Operations") {
  SECTION("Negation") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x", 11}});
    sequence.push_back(input_t{{"time", 10}, {"x", 15}});
    sequence.push_back(input_t{{"time", 20}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("not{x >= 12}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 10), 1));
    expected1.add(std::make_pair(interval::left_open(10, 20), -3));

    CHECK(result1 == expected1);
  }

  SECTION("Disjunction") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x", 11}, {"y", 10}});
    sequence.push_back(input_t{{"time", 10}, {"x", 15}, {"y", 12}});
    sequence.push_back(input_t{{"time", 20}, {"x", 13}, {"y", 17}});
    sequence.push_back(input_t{{"time", 42}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("{x} or {y}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 10), 11));
    expected1.add(std::make_pair(interval::left_open(10, 20), 15));
    expected1.add(std::make_pair(interval::left_open(20, 42), 17));

    CHECK(result1 == expected1);
  }

  SECTION("Conjunction") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x", 11}, {"y", 10}});
    sequence.push_back(input_t{{"time", 10}, {"x", 15}, {"y", 12}});
    sequence.push_back(input_t{{"time", 20}, {"x", 13}, {"y", 17}});
    sequence.push_back(input_t{{"time", 42}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("{x} and {y}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 10), 10));
    expected1.add(std::make_pair(interval::left_open(10, 20), 12));
    expected1.add(std::make_pair(interval::left_open(20, 42), 13));

    CHECK(result1 == expected1);
  }

  SECTION("Implication") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x", -11}, {"y", 10}});
    sequence.push_back(input_t{{"time", 10}, {"x", -15}, {"y", 12}});
    sequence.push_back(input_t{{"time", 20}, {"x", -13}, {"y", 17}});
    sequence.push_back(input_t{{"time", 42}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("{x} implies {y}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 10), 11));
    expected1.add(std::make_pair(interval::left_open(10, 20), 15));
    expected1.add(std::make_pair(interval::left_open(20, 42), 17));

    CHECK(result1 == expected1);
  }
}

TEST_CASE("Untimed Temporal Operations") {
  SECTION("Past Always") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x", -11}, {"y", 10}});
    sequence.push_back(input_t{{"time", 10}, {"x", -15}, {"y", 9}});
    sequence.push_back(input_t{{"time", 20}, {"x", -13}, {"y", 7}});
    sequence.push_back(input_t{{"time", 42}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("historically{x}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 10), -11));
    expected1.add(std::make_pair(interval::left_open(10, 20), -15));
    expected1.add(std::make_pair(interval::left_open(20, 42), -15));

    CHECK(result1 == expected1);
  }

  SECTION("Once") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x", 11}, {"y", 10}});
    sequence.push_back(input_t{{"time", 10}, {"x", 15}, {"y", 9}});
    sequence.push_back(input_t{{"time", 20}, {"x", 13}, {"y", 7}});
    sequence.push_back(input_t{{"time", 42}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("once{x}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 10), 11));
    expected1.add(std::make_pair(interval::left_open(10, 20), 15));
    expected1.add(std::make_pair(interval::left_open(20, 42), 15));

    CHECK(result1 == expected1);
  }

  SECTION("Since") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x", 3}, {"y", 0}});
    sequence.push_back(input_t{{"time", 1}, {"x", 4}, {"y", 1}});
    sequence.push_back(input_t{{"time", 2}, {"x", 5}, {"y", 0}});
    sequence.push_back(input_t{{"time", 3}, {"x", 1}, {"y", 0}});
    sequence.push_back(input_t{{"time", 4}, {"x", -3}, {"y", 0}});
    sequence.push_back(input_t{{"time", 5}, {"x", -1}, {"y", 0}});
    sequence.push_back(input_t{{"time", 6}, {"x", 1}, {"y", 0}});
    sequence.push_back(input_t{{"time", 7}, {"x", 4}, {"y", 1}});
    sequence.push_back(input_t{{"time", 8}, {"x", -1}, {"y", 0}});
    sequence.push_back(input_t{{"time", 9}, {"x", 1}, {"y", 0}});
    sequence.push_back(input_t{{"time", 10}, {"x", 6}, {"y", 0}});
    sequence.push_back(input_t{{"time", 11}, {"x", 1}, {"y", 0}});
    sequence.push_back(input_t{{"time", 12}, {"x", 1}, {"y", 5}});
    sequence.push_back(input_t{{"time", 13}, {"x", 1}, {"y", 0}});
    sequence.push_back(input_t{{"time", 14}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("{x} since {y}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      // std::cout << net1.output() << std::endl;
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();

    expected1.add(std::make_pair(interval::left_open(0, 1), 0));
    expected1.add(std::make_pair(interval::left_open(1, 2), 1));
    expected1.add(std::make_pair(interval::left_open(2, 3), 1));
    expected1.add(std::make_pair(interval::left_open(3, 4), 1));
    expected1.add(std::make_pair(interval::left_open(4, 5), 0));
    expected1.add(std::make_pair(interval::left_open(5, 6), 0));
    expected1.add(std::make_pair(interval::left_open(6, 7), 0));
    expected1.add(std::make_pair(interval::left_open(7, 8), 1));
    expected1.add(std::make_pair(interval::left_open(8, 9), 0));
    expected1.add(std::make_pair(interval::left_open(9, 10), 0));
    expected1.add(std::make_pair(interval::left_open(10, 11), 0));
    expected1.add(std::make_pair(interval::left_open(11, 12), 0));
    expected1.add(std::make_pair(interval::left_open(12, 13), 5));
    expected1.add(std::make_pair(interval::left_open(13, 14), 1));

    CHECK(result1 == expected1);
  }
}

TEST_CASE("Timed Temporal Operations") {
  SECTION("Timed Past Always") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x", 11}});
    sequence.push_back(input_t{{"time", 10}, {"x", 9}});
    sequence.push_back(input_t{{"time", 20}, {"x", 13}});
    sequence.push_back(input_t{{"time", 30}, {"x", 17}});
    sequence.push_back(input_t{{"time", 60}, {"x", 15}});
    sequence.push_back(input_t{{"time", 70}, {"x", 4}});
    sequence.push_back(input_t{{"time", 90}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("historically[12:24]{x}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 12), top));
    expected1.add(std::make_pair(interval::left_open(12, 22), 11));
    expected1.add(std::make_pair(interval::left_open(20, 44), 9));
    expected1.add(std::make_pair(interval::left_open(44, 54), 13));
    expected1.add(std::make_pair(interval::left_open(54, 72), 17));
    expected1.add(std::make_pair(interval::left_open(72, 82), 15));
    expected1.add(std::make_pair(interval::left_open(82, 90), 4));

    CHECK(result1 == expected1);
  }

  SECTION("Timed Past Always Inf") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x", 11}});
    sequence.push_back(input_t{{"time", 10}, {"x", 9}});
    sequence.push_back(input_t{{"time", 20}, {"x", 13}});
    sequence.push_back(input_t{{"time", 30}, {"x", 17}});
    sequence.push_back(input_t{{"time", 60}, {"x", 15}});
    sequence.push_back(input_t{{"time", 70}, {"x", 4}});
    sequence.push_back(input_t{{"time", 90}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("historically[15:]{x}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 15), top));
    expected1.add(std::make_pair(interval::left_open(15, 25), 11));
    expected1.add(std::make_pair(interval::left_open(25, 85), 9));
    expected1.add(std::make_pair(interval::left_open(85, 90), 4));

    CHECK(result1 == expected1);
  }

  SECTION("Timed Once") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x", 11}});
    sequence.push_back(input_t{{"time", 10}, {"x", 9}});
    sequence.push_back(input_t{{"time", 20}, {"x", 13}});
    sequence.push_back(input_t{{"time", 30}, {"x", 17}});
    sequence.push_back(input_t{{"time", 60}, {"x", 15}});
    sequence.push_back(input_t{{"time", 70}, {"x", 4}});
    sequence.push_back(input_t{{"time", 90}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("once[12:24]{x}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 12), bot));
    expected1.add(std::make_pair(interval::left_open(12, 32), 11));
    expected1.add(std::make_pair(interval::left_open(32, 42), 13));
    expected1.add(std::make_pair(interval::left_open(42, 84), 17));
    expected1.add(std::make_pair(interval::left_open(84, 90), 15));

    CHECK(result1 == expected1);
  }

  SECTION("Timed Once Inf") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x", 11}});
    sequence.push_back(input_t{{"time", 10}, {"x", 9}});
    sequence.push_back(input_t{{"time", 20}, {"x", 13}});
    sequence.push_back(input_t{{"time", 30}, {"x", 17}});
    sequence.push_back(input_t{{"time", 60}, {"x", 15}});
    sequence.push_back(input_t{{"time", 70}, {"x", 4}});
    sequence.push_back(input_t{{"time", 90}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("once[15:]{x}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 15), bot));
    expected1.add(std::make_pair(interval::left_open(15, 35), 11));
    expected1.add(std::make_pair(interval::left_open(35, 45), 13));
    expected1.add(std::make_pair(interval::left_open(45, 90), 17));

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x", 3}, {"y", 0}});
    sequence.push_back(input_t{{"time", 1}, {"x", 4}, {"y", 1}});
    sequence.push_back(input_t{{"time", 2}, {"x", 5}, {"y", 0}});
    sequence.push_back(input_t{{"time", 3}, {"x", 1}, {"y", 0}});
    sequence.push_back(input_t{{"time", 4}, {"x", -3}, {"y", 0}});
    sequence.push_back(input_t{{"time", 5}, {"x", -1}, {"y", 0}});
    sequence.push_back(input_t{{"time", 6}, {"x", 1}, {"y", 0}});
    sequence.push_back(input_t{{"time", 7}, {"x", 4}, {"y", 1}});
    sequence.push_back(input_t{{"time", 8}, {"x", -1}, {"y", 0}});
    sequence.push_back(input_t{{"time", 9}, {"x", 1}, {"y", 0}});
    sequence.push_back(input_t{{"time", 10}, {"x", 6}, {"y", 0}});
    sequence.push_back(input_t{{"time", 11}, {"x", 1}, {"y", 0}});
    sequence.push_back(input_t{{"time", 12}, {"x", 1}, {"y", 5}});
    sequence.push_back(input_t{{"time", 13}, {"x", 1}, {"y", 0}});
    sequence.push_back(input_t{{"time", 14}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("{x>0} since[:100] {y>0}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      // std::cout << net1.output() << std::endl;
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();

    expected1.add(std::make_pair(interval::left_open(0, 1), 0));
    expected1.add(std::make_pair(interval::left_open(1, 2), 1));
    expected1.add(std::make_pair(interval::left_open(2, 3), 1));
    expected1.add(std::make_pair(interval::left_open(3, 4), 1));
    expected1.add(std::make_pair(interval::left_open(4, 5), 0));
    expected1.add(std::make_pair(interval::left_open(5, 6), 0));
    expected1.add(std::make_pair(interval::left_open(6, 7), 0));
    expected1.add(std::make_pair(interval::left_open(7, 8), 1));
    expected1.add(std::make_pair(interval::left_open(8, 9), 0));
    expected1.add(std::make_pair(interval::left_open(9, 10), 0));
    expected1.add(std::make_pair(interval::left_open(10, 11), 0));
    expected1.add(std::make_pair(interval::left_open(11, 12), 0));
    expected1.add(std::make_pair(interval::left_open(12, 13), 5));
    expected1.add(std::make_pair(interval::left_open(13, 14), 1));

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since 2") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x", -3}, {"y", -6}});
    sequence.push_back(input_t{{"time", 10}, {"x", -4}, {"y", 1}});
    sequence.push_back(input_t{{"time", 20}, {"x", -5}, {"y", -8}});
    sequence.push_back(input_t{{"time", 30}, {"x", 15}, {"y", 3}});
    sequence.push_back(input_t{{"time", 40}, {"x", -3}, {"y", 22}});
    sequence.push_back(input_t{{"time", 50}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("{x} since[5:15] {y}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();

    expected1.add(std::make_pair(interval::left_open(0, 5), bot));
    expected1.add(std::make_pair(interval::left_open(5, 15), -6));
    expected1.add(std::make_pair(interval::left_open(15, 20), 1));
    expected1.add(std::make_pair(interval::left_open(20, 35), -5));
    expected1.add(std::make_pair(interval::left_open(35, 45), 3));
    expected1.add(std::make_pair(interval::left_open(45, 50), 22));

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since Inf") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x", -3}, {"y", -6}});
    sequence.push_back(input_t{{"time", 1}, {"x", -4}, {"y", 1}});
    sequence.push_back(input_t{{"time", 2}, {"x", -5}, {"y", -8}});
    sequence.push_back(input_t{{"time", 3}, {"x", 15}, {"y", 3}});
    sequence.push_back(input_t{{"time", 4}, {"x", -3}, {"y", 22}});
    sequence.push_back(input_t{{"time", 5}});

    auto net1 = reelay::dense_timed_robustness_0_network<
        time_type, value_t, input_t>::make("{x} since[2:] {y}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = robustness_map();

    expected1.add(std::make_pair(interval::left_open(0, 1), bot));
    expected1.add(std::make_pair(interval::left_open(1, 2), bot));
    expected1.add(std::make_pair(interval::left_open(2, 3), -6));
    expected1.add(std::make_pair(interval::left_open(3, 4), -5));
    expected1.add(std::make_pair(interval::left_open(4, 5), -5));

    CHECK(result1 == expected1);
  }
}
/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <iostream>
#include <vector>
//
#include "catch.hpp"
//
#include "reelay/common.hpp"
#include "reelay/json.hpp"
//
#include "reelay/networks/discrete_timed_robustness_network.hpp"
#include "reelay/options.hpp"

using time_type = int64_t;
using input_t = reelay::json;
using value_t = int64_t;

value_t top = reelay::infinity<value_t>::value();
value_t bot = -reelay::infinity<value_t>::value();

TEST_CASE("Atoms") {
  SECTION("Proposition") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 4}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 5}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("{x1}");

    auto result = std::vector<value_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_t>({3, 4, 4, 5});

    CHECK(result == expected);
  }

  SECTION("AtomicTrue") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", true}});
    sequence.push_back(input_t{{"x1", true}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", false}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("{x1: true}");

    auto result = std::vector<value_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_t>({top, top, top, bot});

    CHECK(result == expected);
  }

  SECTION("AtomicFalse") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", true}});
    sequence.push_back(input_t{{"x1", true}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", false}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("{x1: false}");

    auto result = std::vector<value_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_t>({bot, bot, bot, top});

    CHECK(result == expected);
  }

  SECTION("AtomicString") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", "a"}});
    sequence.push_back(input_t{{"x1", "b"}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", "c"}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("{x1: b}");

    auto result = std::vector<value_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_t>({bot, top, top, bot});

    CHECK(result == expected);
  }

  SECTION("AtomicNumber") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 1}});
    sequence.push_back(input_t{{"x1", 2}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 4}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("{x1: 2}");

    auto result = std::vector<value_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_t>({bot, top, top, bot});

    CHECK(result == expected);
  }

  SECTION("AtomicAny") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", "1"}});
    sequence.push_back(input_t{{"x2", "2"}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", "4"}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("{x1: *}");

    auto result = std::vector<value_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_t>({top, bot, bot, top});

    CHECK(result == expected);
  }

  SECTION("GreaterThan") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 4}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 5}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("{x1 > 4}");

    auto result = std::vector<value_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_t>({-1, 0, 0, 1});

    CHECK(result == expected);
  }

  SECTION("Record Double Comparison") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 2}});
    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 4}});
    sequence.push_back(input_t{{"x1", 5}});
    sequence.push_back(input_t{{"x1", 6}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("{x1 > 3, x1 < 5}");

    auto result = std::vector<value_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_t>({-1, 0, 1, 0, -1});

    CHECK(result == expected);
  }

  SECTION("GreaterEqual") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 4}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 5}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("{x1 >= 4}");

    auto result = std::vector<value_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_t>({-1, 0, 0, 1});

    CHECK(result == expected);
  }

  SECTION("LessThan") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 4}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 5}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("{x1 < 4}");

    auto result = std::vector<value_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_t>({1, 0, 0, -1});

    CHECK(result == expected);
  }

  SECTION("LessEqual") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 4}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 5}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("{x1 <= 4}");

    auto result = std::vector<value_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_t>({1, 0, 0, -1});

    CHECK(result == expected);
  }

  SECTION("Equal") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 4}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 5}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("{x1 == 4}");

    auto result = std::vector<value_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_t>({-1, 0, 0, -1});

    CHECK(result == expected);
  }

  SECTION("NotEqual") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 4}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 5}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("{x1 != 4}");

    auto result = std::vector<value_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_t>({1, 0, 0, 1});

    CHECK(result == expected);
  }
}

TEST_CASE("Boolean Operations") {
  SECTION("Disjunction") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 0}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 0}, {"p2", 3}});
    sequence.push_back(input_t{{"p1", 5}, {"p2", 4}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("{p1>0} or {p2>0}");

    auto result = std::vector<value_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_t>({0, 1, 3, 5});

    CHECK(result == expected);
  }

  SECTION("Conjunction") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 0}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 0}, {"p2", 3}});
    sequence.push_back(input_t{{"p1", 5}, {"p2", 4}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("{p1>0} and {p2>0}");

    auto result = std::vector<value_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_t>({0, 0, 0, 4});

    CHECK(result == expected);
  }

  SECTION("Implication") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 0}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 0}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 1}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("{p1>0} -> {p2>0}");

    auto result = std::vector<value_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_t>({0, 0, 1, 1});

    CHECK(result == expected);
  }

  SECTION("Negation") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", -10}});
    sequence.push_back(input_t{{"p1", 10}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("not{p1>0}");

    auto result = std::vector<value_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_t>({10, -10});

    CHECK(result == expected);
  }
}

TEST_CASE("Untimed Temporal Operations") {
  SECTION("Previous") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 8}, {"p2", -1}});
    sequence.push_back(input_t{{"p1", 7}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", 5}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 6}, {"p2", 0}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("pre{p1>0}");
    auto net2 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("pre{p2>0}");

    auto result1 = std::vector<value_t>();
    auto result2 = std::vector<value_t>();

    for (const auto &s : sequence) {
      net1.update(s);
      net2.update(s);
      result1.push_back(net1.output());
      result2.push_back(net2.output());
    }

    auto expected1 = std::vector<value_t>({bot, 8, 7, 5});
    auto expected2 = std::vector<value_t>({bot, -1, 1, 0});

    CHECK(result1 == expected1);
    CHECK(result2 == expected2);
  }

  SECTION("Always") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 8}, {"p2", -1}});
    sequence.push_back(input_t{{"p1", 7}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", 5}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 6}, {"p2", 0}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("historically{p1>0}");
    auto net2 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("historically{p2>0}");

    auto result1 = std::vector<value_t>();
    auto result2 = std::vector<value_t>();

    for (const auto &s : sequence) {
      net1.update(s);
      net2.update(s);
      result1.push_back(net1.output());
      result2.push_back(net2.output());
    }

    auto expected1 = std::vector<value_t>({8, 7, 5, 5});
    auto expected2 = std::vector<value_t>({-1, -1, -1, -1});

    CHECK(result1 == expected1);
    CHECK(result2 == expected2);
  }

  SECTION("Once") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 0}, {"p2", 4}});
    sequence.push_back(input_t{{"p1", 6}, {"p2", 5}});
    sequence.push_back(input_t{{"p1", 7}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", 5}, {"p2", 8}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("once{p1>0}");
    auto net2 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("once{p2>0}");

    auto result1 = std::vector<value_t>();
    auto result2 = std::vector<value_t>();

    for (const auto &s : sequence) {
      net1.update(s);
      net2.update(s);
      result1.push_back(net1.output());
      result2.push_back(net2.output());
    }

    auto expected1 = std::vector<value_t>({0, 6, 7, 7});
    auto expected2 = std::vector<value_t>({4, 5, 5, 8});

    CHECK(result1 == expected1);
    CHECK(result2 == expected2);
  }

  SECTION("Since") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 3}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 4}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", 5}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", -3}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", -1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 4}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", -1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 6}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 5}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("{p1>0} since {p2>0}");

    auto result1 = std::vector<value_t>();

    for (const auto &s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1
        = std::vector<value_t>({0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 5, 1});

    CHECK(result1 == expected1);
  }

  SECTION("Since") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 3}, {"p2", -120}});
    sequence.push_back(input_t{{"p1", 4}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", 5}, {"p2", -100}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_t{{"p1", -3}, {"p2", -100}});
    sequence.push_back(input_t{{"p1", -1}, {"p2", -100}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_t{{"p1", 4}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", -1}, {"p2", -100}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_t{{"p1", -4}, {"p2", -100}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 5}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", -100}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("{p1>0} since[:100] {p2>0}");

    auto result1 = std::vector<value_t>();

    for (const auto &s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<value_t>(
        {-120, 1, 1, 1, -3, -3, -3, 1, -1, -1, -4, -4, 5, 1, 1, 1});

    CHECK(result1 == expected1);
  }
}

TEST_CASE("Timed Temporal Operations") {
  SECTION("Timed Once") {
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

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("once[2:4]{p1>0}");

    auto result1 = std::vector<value_t>();

    for (const auto &s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1
        = std::vector<value_t>({bot, bot, 3, 4, 5, 5, 5, 2, 2, 14, 14});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Once Zero") {
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

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("once[:3]{p1>0}");

    auto result1 = std::vector<value_t>();

    for (const auto &s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1
        = std::vector<value_t>({3, 4, 5, 5, 5, 5, 2, 14, 14, 14, 14});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Once Inf") {
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

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("once[2:]{p1>0}");

    auto result1 = std::vector<value_t>();

    for (const auto &s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1
        = std::vector<value_t>({bot, bot, 3, 4, 5, 5, 5, 5, 5, 14, 14});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Always") {
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

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("historically[2:4]{p1>0}");

    auto result1 = std::vector<value_t>();

    for (const auto &s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1
        = std::vector<value_t>({top, top, 3, 3, 3, 1, 1, -1, -2, -2, -2});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Always Zero") {
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

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("historically[:3]{p1>0}");

    auto result1 = std::vector<value_t>();

    for (const auto &s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1
        = std::vector<value_t>({3, 3, 3, 1, 1, -1, -2, -2, -2, -2, 2});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Always Inf") {
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

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("historically[2:]{p1>0}");

    auto result1 = std::vector<value_t>();

    for (const auto &s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1
        = std::vector<value_t>({top, top, 3, 3, 3, 1, 1, -1, -2, -2, -2});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 3}, {"p2", -120}});
    sequence.push_back(input_t{{"p1", 4}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", 5}, {"p2", -100}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_t{{"p1", -3}, {"p2", -100}});
    sequence.push_back(input_t{{"p1", -1}, {"p2", -100}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_t{{"p1", 4}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", -1}, {"p2", -100}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_t{{"p1", -4}, {"p2", -100}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 5}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", -100}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("{p1>0} since[2:4] {p2>0}");

    auto result1 = std::vector<value_t>();

    for (const auto &s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<value_t>(
        {bot, bot, -120, 1, -3, -3, -100, -100, -100, -1, -4, -4, -100, -100, 1,
         1});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since Zero") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 3}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 4}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", 5}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", -3}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", -1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 4}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", -1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 6}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 5}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("{p1>0} since[:4] {p2>0}");

    auto result1 = std::vector<value_t>();

    for (const auto &s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1
        = std::vector<value_t>({0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 5, 1});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since Zero 2") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 3}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 4}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", 5}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", -3}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", -1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 4}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", -1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 6}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 5}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("{p1>0} since[:100] {p2>0}");

    auto result1 = std::vector<value_t>();

    for (const auto &s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1
        = std::vector<value_t>({0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 5, 1});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since Inf") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", -3}, {"p2", -6}});
    sequence.push_back(input_t{{"p1", -4}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", -5}, {"p2", -8}});
    sequence.push_back(input_t{{"p1", 15}, {"p2", 3}});
    sequence.push_back(input_t{{"p1", -3}, {"p2", 22}});

    auto net1 = reelay::discrete_timed_robustness_network<
        time_type, value_t, input_t>::make("{p1>0} since[2:] {p2>0}");

    auto result1 = std::vector<value_t>();

    for (const auto &s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<value_t>({bot, bot, -6, -5, -5});

    CHECK(result1 == expected1);
  }
}

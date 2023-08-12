/*
 * Copyright (c) 2019-2023 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "reelay/json.hpp"
#include "reelay/networks/discrete_timed_robustness_network.hpp"

#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <vector>

using time_type = int64_t;
using input_type = reelay::json;
using value_type = int64_t;

TEST_CASE(  // NOLINT(readability-function-cognitive-complexity)
  "Discrete Timed Robustness Atoms",
  "[discrete_timed_robustness]")
{
  value_type top = reelay::infinity<value_type>::value();
  value_type bot = -reelay::infinity<value_type>::value();

  SECTION("Proposition")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", 3}});
    sequence.push_back(input_type{{"x1", 4}});
    sequence.emplace_back(input_type{});
    sequence.push_back(input_type{{"x1", 5}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("{x1}");

    auto result = std::vector<value_type>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_type>({3, 4, 4, 5});

    CHECK(result == expected);
  }

  SECTION("AtomicTrue")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", true}});
    sequence.push_back(input_type{{"x1", true}});
    sequence.emplace_back(input_type{});
    sequence.push_back(input_type{{"x1", false}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("{x1: true}");

    auto result = std::vector<value_type>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_type>({top, top, top, bot});

    CHECK(result == expected);
  }

  SECTION("AtomicFalse")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", true}});
    sequence.push_back(input_type{{"x1", true}});
    sequence.emplace_back(input_type{});
    sequence.push_back(input_type{{"x1", false}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("{x1: false}");

    auto result = std::vector<value_type>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_type>({bot, bot, bot, top});

    CHECK(result == expected);
  }

  SECTION("AtomicString")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", "a"}});
    sequence.push_back(input_type{{"x1", "b"}});
    sequence.emplace_back(input_type{});
    sequence.push_back(input_type{{"x1", "c"}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("{x1: b}");

    auto result = std::vector<value_type>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_type>({bot, top, top, bot});

    CHECK(result == expected);
  }

  SECTION("AtomicNumber")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", 1}});
    sequence.push_back(input_type{{"x1", 2}});
    sequence.emplace_back(input_type{});
    sequence.push_back(input_type{{"x1", 4}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("{x1: 2}");

    auto result = std::vector<value_type>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_type>({bot, top, top, bot});

    CHECK(result == expected);
  }

  SECTION("AtomicAny")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", "1"}});
    sequence.push_back(input_type{{"x2", "2"}});
    sequence.emplace_back(input_type{});
    sequence.push_back(input_type{{"x1", "4"}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("{x1: *}");

    auto result = std::vector<value_type>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_type>({top, bot, bot, top});

    CHECK(result == expected);
  }

  SECTION("GreaterThan")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", 3}});
    sequence.push_back(input_type{{"x1", 4}});
    sequence.emplace_back(input_type{});
    sequence.push_back(input_type{{"x1", 5}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("{x1 > 4}");

    auto result = std::vector<value_type>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_type>({-1, 0, 0, 1});

    CHECK(result == expected);
  }

  SECTION("Record Double Comparison")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", 2}});
    sequence.push_back(input_type{{"x1", 3}});
    sequence.push_back(input_type{{"x1", 4}});
    sequence.push_back(input_type{{"x1", 5}});
    sequence.push_back(input_type{{"x1", 6}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("{x1 > 3, x1 < 5}");

    auto result = std::vector<value_type>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_type>({-1, 0, 1, 0, -1});

    CHECK(result == expected);
  }

  SECTION("GreaterEqual")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", 3}});
    sequence.push_back(input_type{{"x1", 4}});
    sequence.emplace_back(input_type{});
    sequence.push_back(input_type{{"x1", 5}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("{x1 >= 4}");

    auto result = std::vector<value_type>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_type>({-1, 0, 0, 1});

    CHECK(result == expected);
  }

  SECTION("LessThan")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", 3}});
    sequence.push_back(input_type{{"x1", 4}});
    sequence.emplace_back(input_type{});
    sequence.push_back(input_type{{"x1", 5}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("{x1 < 4}");

    auto result = std::vector<value_type>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_type>({1, 0, 0, -1});

    CHECK(result == expected);
  }

  SECTION("LessEqual")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", 3}});
    sequence.push_back(input_type{{"x1", 4}});
    sequence.emplace_back(input_type{});
    sequence.push_back(input_type{{"x1", 5}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("{x1 <= 4}");

    auto result = std::vector<value_type>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_type>({1, 0, 0, -1});

    CHECK(result == expected);
  }

  SECTION("Equal")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", 3}});
    sequence.push_back(input_type{{"x1", 4}});
    sequence.emplace_back(input_type{});
    sequence.push_back(input_type{{"x1", 5}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("{x1 == 4}");

    auto result = std::vector<value_type>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_type>({-1, 0, 0, -1});

    CHECK(result == expected);
  }

  SECTION("NotEqual")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", 3}});
    sequence.push_back(input_type{{"x1", 4}});
    sequence.emplace_back(input_type{});
    sequence.push_back(input_type{{"x1", 5}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("{x1 != 4}");

    auto result = std::vector<value_type>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_type>({1, 0, 0, 1});

    CHECK(result == expected);
  }
}

TEST_CASE(  // NOLINT(readability-function-cognitive-complexity)
  " Discrete Timed Robustness Boolean Operations",
  "[discrete_timed_robustness]")
{
  value_type top = reelay::infinity<value_type>::value();
  value_type bot = -reelay::infinity<value_type>::value();

  SECTION("Disjunction")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", 0}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 0}, {"p2", 3}});
    sequence.push_back(input_type{{"p1", 5}, {"p2", 4}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("{p1>0} or {p2>0}");

    auto result = std::vector<value_type>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_type>({0, 1, 3, 5});

    CHECK(result == expected);
  }

  SECTION("Conjunction")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", 0}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 0}, {"p2", 3}});
    sequence.push_back(input_type{{"p1", 5}, {"p2", 4}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("{p1>0} and {p2>0}");

    auto result = std::vector<value_type>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_type>({0, 0, 0, 4});

    CHECK(result == expected);
  }

  SECTION("Implication")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", 0}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 0}, {"p2", 1}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 1}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("{p1>0} -> {p2>0}");

    auto result = std::vector<value_type>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_type>({0, 0, 1, 1});

    CHECK(result == expected);
  }

  SECTION("Negation")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", -10}});
    sequence.push_back(input_type{{"p1", 10}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("not{p1>0}");

    auto result = std::vector<value_type>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<value_type>({10, -10});

    CHECK(result == expected);
  }
}

TEST_CASE(  // NOLINT(readability-function-cognitive-complexity)
  "Discrete Timed Robustness Temporal Operations (Untimed)",
  "[discrete_timed_robustness]")
{
  value_type top = reelay::infinity<value_type>::value();
  value_type bot = -reelay::infinity<value_type>::value();

  SECTION("Previous")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", 8}, {"p2", -1}});
    sequence.push_back(input_type{{"p1", 7}, {"p2", 1}});
    sequence.push_back(input_type{{"p1", 5}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 6}, {"p2", 0}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("pre{p1>0}");
    auto net2 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("pre{p2>0}");

    auto result1 = std::vector<value_type>();
    auto result2 = std::vector<value_type>();

    for(const auto& s : sequence) {
      net1.update(s);
      net2.update(s);
      result1.push_back(net1.output());
      result2.push_back(net2.output());
    }

    auto expected1 = std::vector<value_type>({bot, 8, 7, 5});
    auto expected2 = std::vector<value_type>({bot, -1, 1, 0});

    CHECK(result1 == expected1);
    CHECK(result2 == expected2);
  }

  SECTION("Always")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", 8}, {"p2", -1}});
    sequence.push_back(input_type{{"p1", 7}, {"p2", 1}});
    sequence.push_back(input_type{{"p1", 5}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 6}, {"p2", 0}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("historically{p1>0}");
    auto net2 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("historically{p2>0}");

    auto result1 = std::vector<value_type>();
    auto result2 = std::vector<value_type>();

    for(const auto& s : sequence) {
      net1.update(s);
      net2.update(s);
      result1.push_back(net1.output());
      result2.push_back(net2.output());
    }

    auto expected1 = std::vector<value_type>({8, 7, 5, 5});
    auto expected2 = std::vector<value_type>({-1, -1, -1, -1});

    CHECK(result1 == expected1);
    CHECK(result2 == expected2);
  }

  SECTION("Once")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", 0}, {"p2", 4}});
    sequence.push_back(input_type{{"p1", 6}, {"p2", 5}});
    sequence.push_back(input_type{{"p1", 7}, {"p2", 1}});
    sequence.push_back(input_type{{"p1", 5}, {"p2", 8}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("once{p1>0}");
    auto net2 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("once{p2>0}");

    auto result1 = std::vector<value_type>();
    auto result2 = std::vector<value_type>();

    for(const auto& s : sequence) {
      net1.update(s);
      net2.update(s);
      result1.push_back(net1.output());
      result2.push_back(net2.output());
    }

    auto expected1 = std::vector<value_type>({0, 6, 7, 7});
    auto expected2 = std::vector<value_type>({4, 5, 5, 8});

    CHECK(result1 == expected1);
    CHECK(result2 == expected2);
  }

  SECTION("Since")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", 3}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 4}, {"p2", 1}});
    sequence.push_back(input_type{{"p1", 5}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", -3}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", -1}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 4}, {"p2", 1}});
    sequence.push_back(input_type{{"p1", -1}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 6}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 5}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 0}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("{p1>0} since {p2>0}");

    auto result1 = std::vector<value_type>();

    for(const auto& s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 =
      std::vector<value_type>({0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 5, 1});

    CHECK(result1 == expected1);
  }

  SECTION("Since")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", 3}, {"p2", -120}});
    sequence.push_back(input_type{{"p1", 4}, {"p2", 1}});
    sequence.push_back(input_type{{"p1", 5}, {"p2", -100}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_type{{"p1", -3}, {"p2", -100}});
    sequence.push_back(input_type{{"p1", -1}, {"p2", -100}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_type{{"p1", 4}, {"p2", 1}});
    sequence.push_back(input_type{{"p1", -1}, {"p2", -100}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_type{{"p1", -4}, {"p2", -100}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 5}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", -100}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("{p1>0} since[:100] {p2>0}");

    auto result1 = std::vector<value_type>();

    for(const auto& s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<value_type>(
      {-120, 1, 1, 1, -3, -3, -3, 1, -1, -1, -4, -4, 5, 1, 1, 1});

    CHECK(result1 == expected1);
  }
}

TEST_CASE(  // NOLINT(readability-function-cognitive-complexity)
  "Discrete Timed Robustness Temporal Operations (Bounded)",
  "[discrete_timed_robustness]")
{
  value_type top = reelay::infinity<value_type>::value();
  value_type bot = -reelay::infinity<value_type>::value();

  SECTION("Timed Once")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", 3}});
    sequence.push_back(input_type{{"p1", 4}});
    sequence.push_back(input_type{{"p1", 5}});
    sequence.push_back(input_type{{"p1", 1}});
    sequence.push_back(input_type{{"p1", 2}});
    sequence.push_back(input_type{{"p1", -1}});
    sequence.push_back(input_type{{"p1", -2}});
    sequence.push_back(input_type{{"p1", 14}});
    sequence.push_back(input_type{{"p1", 4}});
    sequence.push_back(input_type{{"p1", 4}});
    sequence.push_back(input_type{{"p1", 2}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("once[2:4]{p1>0}");

    auto result1 = std::vector<value_type>();

    for(const auto& s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 =
      std::vector<value_type>({bot, bot, 3, 4, 5, 5, 5, 2, 2, 14, 14});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Once Zero")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", 3}});
    sequence.push_back(input_type{{"p1", 4}});
    sequence.push_back(input_type{{"p1", 5}});
    sequence.push_back(input_type{{"p1", 1}});
    sequence.push_back(input_type{{"p1", 2}});
    sequence.push_back(input_type{{"p1", -1}});
    sequence.push_back(input_type{{"p1", -2}});
    sequence.push_back(input_type{{"p1", 14}});
    sequence.push_back(input_type{{"p1", 4}});
    sequence.push_back(input_type{{"p1", 4}});
    sequence.push_back(input_type{{"p1", 2}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("once[:3]{p1>0}");

    auto result1 = std::vector<value_type>();

    for(const auto& s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 =
      std::vector<value_type>({3, 4, 5, 5, 5, 5, 2, 14, 14, 14, 14});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Once Inf")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", 3}});
    sequence.push_back(input_type{{"p1", 4}});
    sequence.push_back(input_type{{"p1", 5}});
    sequence.push_back(input_type{{"p1", 1}});
    sequence.push_back(input_type{{"p1", 2}});
    sequence.push_back(input_type{{"p1", -1}});
    sequence.push_back(input_type{{"p1", -2}});
    sequence.push_back(input_type{{"p1", 14}});
    sequence.push_back(input_type{{"p1", 4}});
    sequence.push_back(input_type{{"p1", 4}});
    sequence.push_back(input_type{{"p1", 2}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("once[2:]{p1>0}");

    auto result1 = std::vector<value_type>();

    for(const auto& s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 =
      std::vector<value_type>({bot, bot, 3, 4, 5, 5, 5, 5, 5, 14, 14});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Always")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", 3}});
    sequence.push_back(input_type{{"p1", 4}});
    sequence.push_back(input_type{{"p1", 5}});
    sequence.push_back(input_type{{"p1", 1}});
    sequence.push_back(input_type{{"p1", 2}});
    sequence.push_back(input_type{{"p1", -1}});
    sequence.push_back(input_type{{"p1", -2}});
    sequence.push_back(input_type{{"p1", 14}});
    sequence.push_back(input_type{{"p1", 4}});
    sequence.push_back(input_type{{"p1", 4}});
    sequence.push_back(input_type{{"p1", 2}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("historically[2:4]{p1>0}");

    auto result1 = std::vector<value_type>();

    for(const auto& s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 =
      std::vector<value_type>({top, top, 3, 3, 3, 1, 1, -1, -2, -2, -2});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Always Zero")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", 3}});
    sequence.push_back(input_type{{"p1", 4}});
    sequence.push_back(input_type{{"p1", 5}});
    sequence.push_back(input_type{{"p1", 1}});
    sequence.push_back(input_type{{"p1", 2}});
    sequence.push_back(input_type{{"p1", -1}});
    sequence.push_back(input_type{{"p1", -2}});
    sequence.push_back(input_type{{"p1", 14}});
    sequence.push_back(input_type{{"p1", 4}});
    sequence.push_back(input_type{{"p1", 4}});
    sequence.push_back(input_type{{"p1", 2}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("historically[:3]{p1>0}");

    auto result1 = std::vector<value_type>();

    for(const auto& s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 =
      std::vector<value_type>({3, 3, 3, 1, 1, -1, -2, -2, -2, -2, 2});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Always Inf")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", 3}});
    sequence.push_back(input_type{{"p1", 4}});
    sequence.push_back(input_type{{"p1", 5}});
    sequence.push_back(input_type{{"p1", 1}});
    sequence.push_back(input_type{{"p1", 2}});
    sequence.push_back(input_type{{"p1", -1}});
    sequence.push_back(input_type{{"p1", -2}});
    sequence.push_back(input_type{{"p1", 14}});
    sequence.push_back(input_type{{"p1", 4}});
    sequence.push_back(input_type{{"p1", 4}});
    sequence.push_back(input_type{{"p1", 2}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("historically[2:]{p1>0}");

    auto result1 = std::vector<value_type>();

    for(const auto& s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 =
      std::vector<value_type>({top, top, 3, 3, 3, 1, 1, -1, -2, -2, -2});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", 3}, {"p2", -120}});
    sequence.push_back(input_type{{"p1", 4}, {"p2", 1}});
    sequence.push_back(input_type{{"p1", 5}, {"p2", -100}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_type{{"p1", -3}, {"p2", -100}});
    sequence.push_back(input_type{{"p1", -1}, {"p2", -100}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_type{{"p1", 4}, {"p2", 1}});
    sequence.push_back(input_type{{"p1", -1}, {"p2", -100}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_type{{"p1", -4}, {"p2", -100}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 5}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", -100}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", -100}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("{p1>0} since[2:4] {p2>0}");

    auto result1 = std::vector<value_type>();

    for(const auto& s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<value_type>(
      {bot,
       bot,
       -120,
       1,
       -3,
       -3,
       -100,
       -100,
       -100,
       -1,
       -4,
       -4,
       -100,
       -100,
       1,
       1});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since Zero")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", 3}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 4}, {"p2", 1}});
    sequence.push_back(input_type{{"p1", 5}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", -3}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", -1}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 4}, {"p2", 1}});
    sequence.push_back(input_type{{"p1", -1}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 6}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 5}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 0}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("{p1>0} since[:4] {p2>0}");

    auto result1 = std::vector<value_type>();

    for(const auto& s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 =
      std::vector<value_type>({0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 5, 1});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since Zero 2")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", 3}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 4}, {"p2", 1}});
    sequence.push_back(input_type{{"p1", 5}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", -3}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", -1}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 4}, {"p2", 1}});
    sequence.push_back(input_type{{"p1", -1}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 6}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 5}});
    sequence.push_back(input_type{{"p1", 1}, {"p2", 0}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("{p1>0} since[:100] {p2>0}");

    auto result1 = std::vector<value_type>();

    for(const auto& s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 =
      std::vector<value_type>({0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 5, 1});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since Inf")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", -3}, {"p2", -6}});
    sequence.push_back(input_type{{"p1", -4}, {"p2", 1}});
    sequence.push_back(input_type{{"p1", -5}, {"p2", -8}});
    sequence.push_back(input_type{{"p1", 15}, {"p2", 3}});
    sequence.push_back(input_type{{"p1", -3}, {"p2", 22}});

    auto net1 = reelay::discrete_timed_robustness_network<
      time_type,
      value_type,
      input_type>::make("{p1>0} since[2:] {p2>0}");

    auto result1 = std::vector<value_type>();

    for(const auto& s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<value_type>({bot, bot, -6, -5, -5});

    CHECK(result1 == expected1);
  }
}

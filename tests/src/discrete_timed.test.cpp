/*
 * Copyright (c) 2019-2023 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "reelay/json.hpp"
#include "reelay/networks/discrete_timed_network.hpp"

#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <string>
#include <vector>

using time_type = int64_t;
using input_type = reelay::json;

TEST_CASE(  // NOLINT(readability-function-cognitive-complexity)
  "Discrete Timed Atoms",
  "[discrete_timed]")
{
  SECTION("AtomicProposition")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", false}});
    sequence.push_back(input_type{{"x1", false}});
    sequence.emplace_back(input_type{});
    sequence.push_back(input_type{{"x1", true}});

    auto net1 =
      reelay::discrete_timed_network<time_type, input_type>::make("{x1}");

    auto result = std::vector<bool>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({false, false, false, true});

    CHECK(result == expected);
  }

  SECTION("AtomicTrue")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", false}});
    sequence.push_back(input_type{{"x1", false}});
    sequence.emplace_back(input_type{});
    sequence.push_back(input_type{{"x1", true}});

    auto net1 =
      reelay::discrete_timed_network<time_type, input_type>::make("{x1:true}");

    auto result = std::vector<bool>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({false, false, false, true});

    CHECK(result == expected);
  }

  SECTION("AtomicFalse")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", false}});
    sequence.push_back(input_type{{"x1", false}});
    sequence.emplace_back(input_type{});
    sequence.push_back(input_type{{"x1", true}});

    auto net1 =
      reelay::discrete_timed_network<time_type, input_type>::make("{x1:false}");

    auto result = std::vector<bool>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({true, true, true, false});

    CHECK(result == expected);
  }

  SECTION("GreaterThan")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", 3}});
    sequence.push_back(input_type{{"x1", 4}});
    sequence.emplace_back(input_type{});
    sequence.push_back(input_type{{"x1", 5}});

    auto net1 =
      reelay::discrete_timed_network<time_type, input_type>::make("{x1 > 4}");

    auto result = std::vector<bool>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({false, false, false, true});

    CHECK(result == expected);
  }

  SECTION("GreaterEqual")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", 3}});
    sequence.push_back(input_type{{"x1", 4}});
    sequence.emplace_back(input_type{});
    sequence.push_back(input_type{{"x1", 5}});

    auto net1 =
      reelay::discrete_timed_network<time_type, input_type>::make("{x1 >= 4}");

    auto result = std::vector<bool>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({false, true, true, true});

    CHECK(result == expected);
  }

  SECTION("LessThan")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", 3}});
    sequence.push_back(input_type{{"x1", 4}});
    sequence.emplace_back(input_type{});
    sequence.push_back(input_type{{"x1", 5}});

    auto net1 =
      reelay::discrete_timed_network<time_type, input_type>::make("{x1 < 4}");

    auto result = std::vector<bool>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({true, false, false, false});

    CHECK(result == expected);
  }

  SECTION("LessEqual")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", 3}});
    sequence.push_back(input_type{{"x1", 4}});
    sequence.emplace_back(input_type{});
    sequence.push_back(input_type{{"x1", 5}});

    auto net1 =
      reelay::discrete_timed_network<time_type, input_type>::make("{x1 <= 4}");

    auto result = std::vector<bool>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({true, true, true, false});

    CHECK(result == expected);
  }

  SECTION("AtomicNotEqual")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", 3}});
    sequence.push_back(input_type{{"x1", 4}});
    sequence.emplace_back(input_type{});
    sequence.push_back(input_type{{"x1", 5}});

    auto net1 =
      reelay::discrete_timed_network<time_type, input_type>::make("{x1 != 4}");

    auto result = std::vector<bool>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({true, false, false, true});

    CHECK(result == expected);
  }

  SECTION("AtomicSimpleString")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", "a"}});
    sequence.push_back(input_type{{"x1", "b"}});
    sequence.emplace_back(input_type{});
    sequence.push_back(input_type{{"x1", "c"}});

    auto net1 =
      reelay::discrete_timed_network<time_type, input_type>::make("{x1: a}");

    auto result = std::vector<bool>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({true, false, false, false});

    CHECK(result == expected);
  }
  SECTION("AtomicNumber")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", 3}});
    sequence.push_back(input_type{{"x1", 1}});
    sequence.emplace_back(input_type{});
    sequence.push_back(input_type{{"x1", 3}});

    auto net1 =
      reelay::discrete_timed_network<time_type, input_type>::make("{x1: 3}");

    auto result = std::vector<bool>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({true, false, false, true});

    CHECK(result == expected);
  }

  SECTION("AtomicAny")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back({{"x1", "3"}, {"x2", "a"}});
    sequence.push_back({{"x1", "1"}});
    sequence.push_back({{"x1", "3"}, {"x2", "c"}});

    auto net1 =
      reelay::discrete_timed_network<time_type, input_type>::make("{x2: *}");

    auto result = std::vector<bool>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({true, false, true});

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

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
      "{x1 > 3, x1 < 5}");

    auto result = std::vector<bool>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({false, false, true, false, false});

    CHECK(result == expected);
  }
}

TEST_CASE(  // NOLINT(readability-function-cognitive-complexity)
  "Discrete Timed Boolean Operations",
  "[discrete_timed]")
{
  SECTION("Disjunction")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
      "{p1} or {p2}");

    auto result = std::vector<bool>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({false, true, true, true});

    CHECK(result == expected);
  }

  SECTION("Conjunction")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
      "{p1} and {p2}");

    auto result = std::vector<bool>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({false, false, false, true});

    CHECK(result == expected);
  }

  SECTION("Implication")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
      "{p1} -> {p2}");

    auto result = std::vector<bool>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({true, false, true, true});

    CHECK(result == expected);
  }

  SECTION("Negation")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", false}});
    sequence.push_back(input_type{{"p1", true}});

    auto net1 =
      reelay::discrete_timed_network<time_type, input_type>::make("not {p1}");

    auto result = std::vector<bool>();

    for(const auto& row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({true, false});

    CHECK(result == expected);
  }
}

TEST_CASE(  // NOLINT(readability-function-cognitive-complexity)
  "Discrete Timed Temporal Operations (Untimed)",
  "[discrete_timed]")
{
  SECTION("Previous")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", true}});

    auto net1 =
      reelay::discrete_timed_network<time_type, input_type>::make("pre{p1}");
    auto net2 =
      reelay::discrete_timed_network<time_type, input_type>::make("pre{p2}");

    auto result1 = std::vector<bool>();
    auto result2 = std::vector<bool>();

    for(const auto& s : sequence) {
      net1.update(s);
      net2.update(s);
      result1.push_back(net1.output());
      result2.push_back(net2.output());
    }

    auto expected1 = std::vector<bool>({false, true, false, true});
    auto expected2 = std::vector<bool>({false, false, true, false});

    CHECK(result1 == expected1);
    CHECK(result2 == expected2);
  }

  SECTION("Past Always")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
      "historically{p1}");
    auto net2 = reelay::discrete_timed_network<time_type, input_type>::make(
      "historically{p2}");

    auto result1 = std::vector<bool>();
    auto result2 = std::vector<bool>();

    for(const auto& s : sequence) {
      net1.update(s);
      net2.update(s);
      result1.push_back(net1.output());
      result2.push_back(net2.output());
    }

    auto expected1 = std::vector<bool>({true, true, true, true});
    auto expected2 = std::vector<bool>({true, true, false, false});

    CHECK(result1 == expected1);
    CHECK(result2 == expected2);
  }

  SECTION("Past Sometime")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", false}});

    auto net1 =
      reelay::discrete_timed_network<time_type, input_type>::make("once{p1}");
    auto net2 =
      reelay::discrete_timed_network<time_type, input_type>::make("once{p2}");

    auto result1 = std::vector<bool>();
    auto result2 = std::vector<bool>();

    for(const auto& s : sequence) {
      net1.update(s);
      net2.update(s);
      result1.push_back(net1.output());
      result2.push_back(net2.output());
    }

    auto expected1 = std::vector<bool>({false, false, true, true});
    auto expected2 = std::vector<bool>({false, false, false, false});

    CHECK(result1 == expected1);
    CHECK(result2 == expected2);
  }

  SECTION("Since")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
      "{p1} since {p2}");

    auto result1 = std::vector<bool>();

    for(const auto& s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<bool>(
      {false,
       true,
       true,
       true,
       true,
       false,
       false,
       true,
       true,
       true,
       false,
       false,
       true,
       true});

    CHECK(result1 == expected1);
  }
}

TEST_CASE(  // NOLINT(readability-function-cognitive-complexity)
  "Discrete Timed Timed Temporal Operations (Bounded)",
  "[discrete_timed]")
{
  SECTION("Timed Once")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", false}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
      "once[2:4]{p2}");

    auto result1 = std::vector<bool>();

    for(const auto& s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<bool>(
      {false, false, false, true, true, true, false, false, false, true, true});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Once Zero")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", false}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
      "once[:4]{p2}");

    auto result1 = std::vector<bool>();

    for(const auto& s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<bool>(
      {false, true, true, true, true, true, false, true, true, true, true});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Once Inf")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", false}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
      "once[2:]{p2}");

    auto result1 = std::vector<bool>();

    for(const auto& s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<bool>(
      {false, false, false, true, true, true, true, true, true, true, true});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Always")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", false}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
      "historically[2:4]{p1}");

    auto result1 = std::vector<bool>();

    for(const auto& s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<bool>(
      {true, true, false, false, false, true, true, true, true, true, true});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Always Zero")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", false}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
      "historically[:4]{p1}");

    auto result1 = std::vector<bool>();

    for(const auto& s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<bool>(
      {false, false, false, false, false, true, true, true, true, true, false});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Always Inf")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", false}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
      "historically[2:]{p1}");

    auto result1 = std::vector<bool>();

    for(const auto& s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<bool>(
      {true, true, true, true, true, true, true, true, true, true, false});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", false}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
      "{p1} since[2:4] {p2}");

    auto result1 = std::vector<bool>();

    for(const auto& s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<bool>(
      {false,
       false,
       false,
       true,
       true,
       true,
       false,
       false,
       false,
       true,
       false});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since Zero")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", false}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
      "{p1} since[:4] {p2}");

    auto result1 = std::vector<bool>();

    for(const auto& s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<bool>(
      {false, true, true, true, true, true, false, true, true, true, false});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since Inf")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", false}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
      "{p1} since[2:] {p2}");

    auto result1 = std::vector<bool>();

    for(const auto& s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<bool>(
      {false, false, false, true, true, true, true, true, true, true, false});

    CHECK(result1 == expected1);
  }
}

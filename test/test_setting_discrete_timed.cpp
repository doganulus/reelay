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
#include "reelay/networks/discrete_timed_network.hpp"
#include "reelay/options.hpp"

using time_type = int64_t;
using input_type = reelay::json;

TEST_CASE("Atoms") {
  SECTION("AtomicProposition") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", false}});
    sequence.push_back(input_type{{"x1", false}});
    sequence.push_back(input_type{});
    sequence.push_back(input_type{{"x1", true}});

    auto net1
        = reelay::discrete_timed_network<time_type, input_type>::make("{x1}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({0, 0, 0, 1});

    CHECK(result == expected);
  }

  SECTION("AtomicTrue") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", false}});
    sequence.push_back(input_type{{"x1", false}});
    sequence.push_back(input_type{});
    sequence.push_back(input_type{{"x1", true}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
        "{x1:true}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({0, 0, 0, 1});

    CHECK(result == expected);
  }

  SECTION("AtomicFalse") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", false}});
    sequence.push_back(input_type{{"x1", false}});
    sequence.push_back(input_type{});
    sequence.push_back(input_type{{"x1", true}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
        "{x1:false}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({1, 1, 1, 0});

    CHECK(result == expected);
  }

  SECTION("GreaterThan") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", 3}});
    sequence.push_back(input_type{{"x1", 4}});
    sequence.push_back(input_type{});
    sequence.push_back(input_type{{"x1", 5}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
        "{x1 > 4}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({0, 0, 0, 1});

    CHECK(result == expected);
  }

  SECTION("GreaterEqual") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", 3}});
    sequence.push_back(input_type{{"x1", 4}});
    sequence.push_back(input_type{});
    sequence.push_back(input_type{{"x1", 5}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
        "{x1 >= 4}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({0, 1, 1, 1});

    CHECK(result == expected);
  }

  SECTION("LessThan") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", 3}});
    sequence.push_back(input_type{{"x1", 4}});
    sequence.push_back(input_type{});
    sequence.push_back(input_type{{"x1", 5}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
        "{x1 < 4}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({1, 0, 0, 0});

    CHECK(result == expected);
  }

  SECTION("LessEqual") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", 3}});
    sequence.push_back(input_type{{"x1", 4}});
    sequence.push_back(input_type{});
    sequence.push_back(input_type{{"x1", 5}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
        "{x1 <= 4}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({1, 1, 1, 0});

    CHECK(result == expected);
  }

  SECTION("AtomicNotEqual") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", 3}});
    sequence.push_back(input_type{{"x1", 4}});
    sequence.push_back(input_type{});
    sequence.push_back(input_type{{"x1", 5}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
        "{x1 != 4}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({1, 0, 0, 1});

    CHECK(result == expected);
  }

  SECTION("AtomicSimpleString") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", "a"}});
    sequence.push_back(input_type{{"x1", "b"}});
    sequence.push_back(input_type{});
    sequence.push_back(input_type{{"x1", "c"}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
        "{x1: a}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({1, 0, 0, 0});

    CHECK(result == expected);
  }
  SECTION("AtomicNumber") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", 3}});
    sequence.push_back(input_type{{"x1", 1}});
    sequence.push_back(input_type{});
    sequence.push_back(input_type{{"x1", 3}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
        "{x1: 3}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({1, 0, 0, 1});

    CHECK(result == expected);
  }

  SECTION("AtomicAny") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", "3"}, {"x2", "a"}});
    sequence.push_back(input_type{{"x1", true}});
    sequence.push_back(input_type{{"x1", "3"}, {"x2", "c"}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
        "{x2: *}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({1, 0, 1});

    CHECK(result == expected);
  }

  SECTION("Record Double Comparison") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x1", 2}});
    sequence.push_back(input_type{{"x1", 3}});
    sequence.push_back(input_type{{"x1", 4}});
    sequence.push_back(input_type{{"x1", 5}});
    sequence.push_back(input_type{{"x1", 6}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
        "{x1 > 3, x1 < 5}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({0, 0, 1, 0, 0});

    CHECK(result == expected);
  }
}

TEST_CASE("Boolean Operations") {
  SECTION("Disjunction") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
        "{p1} or {p2}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({0, 1, 1, 1});

    CHECK(result == expected);
  }

  SECTION("Conjuction") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
        "{p1} and {p2}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({0, 0, 0, 1});

    CHECK(result == expected);
  }

  SECTION("Implication") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", true}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
        "{p1} -> {p2}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({1, 0, 1, 1});

    CHECK(result == expected);
  }

  SECTION("Negation") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", false}});
    sequence.push_back(input_type{{"p1", true}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
        "not {p1}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto expected = std::vector<bool>({1, 0});

    CHECK(result == expected);
  }
}

TEST_CASE("Untimed Temporal Operations") {
  SECTION("Previous") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", true}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", true}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
        "pre{p1}");
    auto net2 = reelay::discrete_timed_network<time_type, input_type>::make(
        "pre{p2}");

    auto result1 = std::vector<bool>();
    auto result2 = std::vector<bool>();

    for (const auto &s : sequence) {
      net1.update(s);
      net2.update(s);
      result1.push_back(net1.output());
      result2.push_back(net2.output());
    }

    auto expected1 = std::vector<bool>({0, 1, 0, 1});
    auto expected2 = std::vector<bool>({0, 0, 1, 0});

    CHECK(result1 == expected1);
    CHECK(result2 == expected2);
  }

  SECTION("Past Always") {
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

    for (const auto &s : sequence) {
      net1.update(s);
      net2.update(s);
      result1.push_back(net1.output());
      result2.push_back(net2.output());
    }

    auto expected1 = std::vector<bool>({1, 1, 1, 1});
    auto expected2 = std::vector<bool>({1, 1, 0, 0});

    CHECK(result1 == expected1);
    CHECK(result2 == expected2);
  }

  SECTION("Past Sometime") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"p1", false}, {"p2", false}});

    auto net1 = reelay::discrete_timed_network<time_type, input_type>::make(
        "once{p1}");
    auto net2 = reelay::discrete_timed_network<time_type, input_type>::make(
        "once{p2}");

    auto result1 = std::vector<bool>();
    auto result2 = std::vector<bool>();

    for (const auto &s : sequence) {
      net1.update(s);
      net2.update(s);
      result1.push_back(net1.output());
      result2.push_back(net2.output());
    }

    auto expected1 = std::vector<bool>({0, 0, 1, 1});
    auto expected2 = std::vector<bool>({0, 0, 0, 0});

    CHECK(result1 == expected1);
    CHECK(result2 == expected2);
  }

  SECTION("Since") {
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

    for (const auto &s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1
        = std::vector<bool>({0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1});

    CHECK(result1 == expected1);
  }
}

TEST_CASE("Timed Temporal Operations") {
  SECTION("Timed Once") {
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

    for (const auto &s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<bool>({0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Once Zero") {
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

    for (const auto &s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<bool>({0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Once Inf") {
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

    for (const auto &s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<bool>({0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Always") {
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

    for (const auto &s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<bool>({1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Always Zero") {
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

    for (const auto &s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<bool>({0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Always Inf") {
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

    for (const auto &s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<bool>({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since") {
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

    for (const auto &s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<bool>({0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since Zero") {
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

    for (const auto &s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<bool>({0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since Inf") {
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

    for (const auto &s : sequence) {
      net1.update(s);
      result1.push_back(net1.output());
    }

    auto expected1 = std::vector<bool>({0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0});

    CHECK(result1 == expected1);
  }
}

/*
 * Copyright (c) 2019-2023 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "reelay/intervals.hpp"
#include "reelay/json.hpp"
#include "reelay/networks/dense_timed_network.hpp"
#include "reelay/options.hpp"

#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <vector>

using time_type = double;
using input_type = reelay::json;
using interval = reelay::interval<time_type>;
using interval_set = reelay::interval_set<time_type>;

TEST_CASE(  // NOLINT(readability-function-cognitive-complexity)
  "Dense Timed Atoms",
  "[dense_timed]")
{
  SECTION("Proposition_ZeroHoldForward")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"x1", true}});
    sequence.push_back(input_type{{"time", 3.1}});
    sequence.push_back(input_type{{"time", 3.3}, {"x1", false}});
    sequence.push_back(input_type{{"time", 3.5}, {"x1", false}});
    sequence.push_back(input_type{{"time", 4}});
    sequence.push_back(input_type{{"time", 5.2}, {"x1", true}});
    sequence.push_back(input_type{{"time", 5.5}});

    auto net1 =
      reelay::dense_timed_network<time_type, input_type>::make("{x1}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(0, 3.3));
    expected1.add(interval::left_open(5.2, 5.5));

    CHECK(result1 == expected1);
  }

  SECTION("AtomicAny_ZeroHoldForward")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"x1", true}});
    sequence.push_back(input_type{{"time", 3.1}});
    sequence.push_back(input_type{{"time", 3.3}, {"x1", false}});
    sequence.push_back(input_type{{"time", 3.5}, {"x1", false}});
    sequence.push_back(input_type{{"time", 4}});
    sequence.push_back(input_type{{"time", 5.2}, {"x1", true}});
    sequence.push_back(input_type{{"time", 5.5}});

    auto net1 =
      reelay::dense_timed_network<time_type, input_type>::make("{x1:*}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(0, 3.1));
    expected1.add(interval::left_open(3.3, 4));
    expected1.add(interval::left_open(5.2, 5.5));

    CHECK(result1 == expected1);
  }

  SECTION("BooleanTrue_ZeroHoldForward")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"x1", true}});
    sequence.push_back(input_type{{"time", 3.1}});
    sequence.push_back(input_type{{"time", 3.3}, {"x1", false}});
    sequence.push_back(input_type{{"time", 3.5}, {"x1", false}});
    sequence.push_back(input_type{{"time", 4}});
    sequence.push_back(input_type{{"time", 5.2}, {"x1", true}});
    sequence.push_back(input_type{{"time", 5.5}});

    auto net1 =
      reelay::dense_timed_network<time_type, input_type>::make("{x1: true}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(0, 3.3));
    expected1.add(interval::left_open(5.2, 5.5));

    CHECK(result1 == expected1);
  }

  SECTION("BooleanFalse_ZeroHoldForward")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"x1", true}});
    sequence.push_back(input_type{{"time", 3.1}});
    sequence.push_back(input_type{{"time", 3.3}, {"x1", false}});
    sequence.push_back(input_type{{"time", 3.5}, {"x1", false}});
    sequence.push_back(input_type{{"time", 4}});
    sequence.push_back(input_type{{"time", 5.2}, {"x1", true}});
    sequence.push_back(input_type{{"time", 5.5}});

    auto net1 =
      reelay::dense_timed_network<time_type, input_type>::make("{x1: false}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(3.3, 5.2));

    CHECK(result1 == expected1);
  }

  SECTION("AtomicString_ZeroHoldForward")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"x1", "veritas"}});
    sequence.push_back(input_type{{"time", 3.1}});
    sequence.push_back(input_type{{"time", 3.3}, {"x1", "fortuna"}});
    sequence.push_back(input_type{{"time", 3.5}, {"x1", "fortuna"}});
    sequence.push_back(input_type{{"time", 4}});
    sequence.push_back(input_type{{"time", 5.2}, {"x1", "veritas"}});
    sequence.push_back(input_type{{"time", 5.5}});

    auto net1 =
      reelay::dense_timed_network<time_type, input_type>::make("{x1: veritas}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(0, 3.3));
    expected1.add(interval::left_open(5.2, 5.5));

    CHECK(result1 == expected1);
  }

  SECTION("AtomicStringQuoted_ZeroHoldForward")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"x1", "lorem ipsum"}});
    sequence.push_back(input_type{{"time", 3.1}});
    sequence.push_back(input_type{{"time", 3.3}, {"x1", "not_a_string"}});
    sequence.push_back(input_type{{"time", 3.5}, {"x1", "not_a_string_too"}});
    sequence.push_back(input_type{{"time", 4}});
    sequence.push_back(input_type{{"time", 5.2}, {"x1", "lorem ipsum"}});
    sequence.push_back(input_type{{"time", 5.5}});

    auto net1 = reelay::dense_timed_network<time_type, input_type>::make(
      "{x1: 'lorem ipsum'}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(0, 3.3));
    expected1.add(interval::left_open(5.2, 5.5));

    CHECK(result1 == expected1);
  }

  SECTION("AtomicNumber_ZeroHoldForward")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"x1", 12}});
    sequence.push_back(input_type{{"time", 3.1}});
    sequence.push_back(input_type{{"time", 3.3}, {"x1", 11}});
    sequence.push_back(input_type{{"time", 3.5}, {"x1", 12.5}});
    sequence.push_back(input_type{{"time", 4}});
    sequence.push_back(input_type{{"time", 5.2}, {"x1", 12.5}});
    sequence.push_back(input_type{{"time", 5.5}});

    auto net1 =
      reelay::dense_timed_network<time_type, input_type>::make("{x1: 12.5}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(3.5, 5.5));

    CHECK(result1 == expected1);
  }

  SECTION("LessThan_ZeroHoldForward")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"x1", 3}});
    sequence.push_back(input_type{{"time", 3.1}, {"x1", 3}});
    sequence.push_back(input_type{{"time", 3.3}, {"x1", 5}});
    sequence.push_back(input_type{{"time", 3.5}, {"x1", 4}});
    sequence.push_back(input_type{{"time", 4}});

    auto net1 =
      reelay::dense_timed_network<time_type, input_type>::make("{x1 < 4}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(0, 3.3));

    CHECK(result1 == expected1);
  }

  SECTION("LessEqualThan_ZeroHoldForward")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"x1", 3}});
    sequence.push_back(input_type{{"time", 3.1}, {"x1", 3}});
    sequence.push_back(input_type{{"time", 3.3}, {"x1", 5}});
    sequence.push_back(input_type{{"time", 3.5}, {"x1", 4}});
    sequence.push_back(input_type{{"time", 4}});

    auto net1 =
      reelay::dense_timed_network<time_type, input_type>::make("{x1 <= 4}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(0, 3.3));
    expected1.add(interval::left_open(3.5, 4));

    CHECK(result1 == expected1);
  }

  SECTION("GreaterThan_ZeroHoldForward")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"x1", 3}});
    sequence.push_back(input_type{{"time", 3.1}, {"x1", 3}});
    sequence.push_back(input_type{{"time", 3.3}, {"x1", 5}});
    sequence.push_back(input_type{{"time", 3.5}, {"x1", 4}});
    sequence.push_back(input_type{{"time", 4}});

    auto net1 =
      reelay::dense_timed_network<time_type, input_type>::make("{x1 > 4}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(3.3, 3.5));

    CHECK(result1 == expected1);
  }

  SECTION("GreaterEqualThan_ZeroHoldForward")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"x1", 3}});
    sequence.push_back(input_type{{"time", 3.1}, {"x1", 3}});
    sequence.push_back(input_type{{"time", 3.3}, {"x1", 5}});
    sequence.push_back(input_type{{"time", 3.5}, {"x1", 4}});
    sequence.push_back(input_type{{"time", 4}});

    auto net1 =
      reelay::dense_timed_network<time_type, input_type>::make("{x1 >= 4}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(3.3, 4));

    CHECK(result1 == expected1);
  }

  SECTION("InBetween_ZeroHoldForward")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"x1", 3}});
    sequence.push_back(input_type{{"time", 3.1}, {"x1", 3}});
    sequence.push_back(input_type{{"time", 3.3}, {"x1", 5}});
    sequence.push_back(input_type{{"time", 3.5}, {"x1", 4}});
    sequence.push_back(input_type{{"time", 4}});

    auto net1 = reelay::dense_timed_network<time_type, input_type>::make(
      "{x1 > 3.5, x1 < 4.5}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(3.5, 4));

    CHECK(result1 == expected1);
  }

  SECTION("LessThan_PiecewiseLinear")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"x1", -2}});
    sequence.push_back(input_type{{"time", 1}, {"x1", -2}});
    sequence.push_back(input_type{{"time", 2}, {"x1", -1}});
    sequence.push_back(input_type{{"time", 4}, {"x1", 1}});
    sequence.push_back(input_type{{"time", 5}, {"x1", 2}});
    sequence.push_back(input_type{{"time", 6}, {"x1", 2}});
    sequence.push_back(input_type{{"time", 7}, {"x1", 1}});
    sequence.push_back(input_type{{"time", 9}, {"x1", -1}});
    sequence.push_back(input_type{{"time", 10}, {"x1", -2}});
    sequence.push_back(input_type{{"time", 11}, {"x1", 0}});
    sequence.push_back(input_type{{"time", 12}, {"x1", 0}});
    sequence.push_back(input_type{{"time", 13}, {"x1", 2}});
    sequence.push_back(input_type{{"time", 14}, {"x1", 0}});
    sequence.push_back(input_type{{"time", 15}, {"x1", -2}});

    auto opts =
      reelay::basic_options().with_interpolation(reelay::piecewise::linear);

    auto net1 = reelay::dense_timed_network<time_type, input_type>::make(
      "{x1 < 0}", opts);

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(0, 3));
    expected1.add(interval::left_open(8, 11));
    expected1.add(interval::left_open(14, 15));

    CHECK(result1 == expected1);
  }

  SECTION("LessEqualThan_PiecewiseLinear")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"x1", -2}});
    sequence.push_back(input_type{{"time", 1}, {"x1", -2}});
    sequence.push_back(input_type{{"time", 2}, {"x1", -1}});
    sequence.push_back(input_type{{"time", 4}, {"x1", 1}});
    sequence.push_back(input_type{{"time", 5}, {"x1", 2}});
    sequence.push_back(input_type{{"time", 6}, {"x1", 2}});
    sequence.push_back(input_type{{"time", 7}, {"x1", 1}});
    sequence.push_back(input_type{{"time", 9}, {"x1", -1}});
    sequence.push_back(input_type{{"time", 10}, {"x1", -2}});
    sequence.push_back(input_type{{"time", 11}, {"x1", 0}});
    sequence.push_back(input_type{{"time", 12}, {"x1", 0}});
    sequence.push_back(input_type{{"time", 13}, {"x1", 2}});
    sequence.push_back(input_type{{"time", 14}, {"x1", 0}});
    sequence.push_back(input_type{{"time", 15}, {"x1", -2}});

    auto opts =
      reelay::basic_options().with_interpolation(reelay::piecewise::linear);

    auto net1 = reelay::dense_timed_network<time_type, input_type>::make(
      "{x1 <= 0}", opts);

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(0, 3));
    expected1.add(interval::left_open(8, 12));
    expected1.add(interval::left_open(14, 15));

    CHECK(result1 == expected1);
  }

  SECTION("GreaterThan_PiecewiseLinear")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"x1", -2}});
    sequence.push_back(input_type{{"time", 1}, {"x1", -2}});
    sequence.push_back(input_type{{"time", 2}, {"x1", -1}});
    sequence.push_back(input_type{{"time", 4}, {"x1", 1}});
    sequence.push_back(input_type{{"time", 5}, {"x1", 2}});
    sequence.push_back(input_type{{"time", 6}, {"x1", 2}});
    sequence.push_back(input_type{{"time", 7}, {"x1", 1}});
    sequence.push_back(input_type{{"time", 9}, {"x1", -1}});
    sequence.push_back(input_type{{"time", 10}, {"x1", -2}});
    sequence.push_back(input_type{{"time", 11}, {"x1", 0}});
    sequence.push_back(input_type{{"time", 12}, {"x1", 0}});
    sequence.push_back(input_type{{"time", 13}, {"x1", 2}});
    sequence.push_back(input_type{{"time", 14}, {"x1", 0}});
    sequence.push_back(input_type{{"time", 15}, {"x1", -2}});

    auto opts =
      reelay::basic_options().with_interpolation(reelay::piecewise::linear);

    auto net1 = reelay::dense_timed_network<time_type, input_type>::make(
      "{x1 > 0}", opts);

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(3, 8));
    expected1.add(interval::left_open(12, 14));

    CHECK(result1 == expected1);
  }

  SECTION("GreaterEqualThan_PiecewiseLinear")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"x1", -2}});
    sequence.push_back(input_type{{"time", 1}, {"x1", -2}});
    sequence.push_back(input_type{{"time", 2}, {"x1", -1}});
    sequence.push_back(input_type{{"time", 4}, {"x1", 1}});
    sequence.push_back(input_type{{"time", 5}, {"x1", 2}});
    sequence.push_back(input_type{{"time", 6}, {"x1", 2}});
    sequence.push_back(input_type{{"time", 7}, {"x1", 1}});
    sequence.push_back(input_type{{"time", 9}, {"x1", -1}});
    sequence.push_back(input_type{{"time", 10}, {"x1", -2}});
    sequence.push_back(input_type{{"time", 11}, {"x1", 0}});
    sequence.push_back(input_type{{"time", 12}, {"x1", 0}});
    sequence.push_back(input_type{{"time", 13}, {"x1", 2}});
    sequence.push_back(input_type{{"time", 14}, {"x1", 0}});
    sequence.push_back(input_type{{"time", 15}, {"x1", -2}});

    auto opts =
      reelay::basic_options().with_interpolation(reelay::piecewise::linear);

    auto net1 = reelay::dense_timed_network<time_type, input_type>::make(
      "{x1 >= 0}", opts);

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(3, 8));
    expected1.add(interval::left_open(11, 14));

    CHECK(result1 == expected1);
  }
}

TEST_CASE(  // NOLINT(readability-function-cognitive-complexity)
  "Dense Timed Boolean Operations",
  "[dense_timed]")
{
  SECTION("Disjunction")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 10}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 20}, {"p1", false}, {"p2", true}});
    sequence.push_back(input_type{{"time", 30}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 40}});

    auto net1 =
      reelay::dense_timed_network<time_type, input_type>::make("{p1} or {p2}");

    auto result = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result = result | net1.output();
    }

    auto expected = interval_set();
    expected.add(interval::left_open(10, 40));

    CHECK(result == expected);
  }

  SECTION("Conjunction")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 10}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 20}, {"p1", false}, {"p2", true}});
    sequence.push_back(input_type{{"time", 30}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 40}});

    auto net1 =
      reelay::dense_timed_network<time_type, input_type>::make("{p1} and {p2}");

    auto result = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result = result | net1.output();
    }

    auto expected = interval_set();
    expected.add(interval::left_open(30, 40));

    CHECK(result == expected);
  }

  SECTION("Implication")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 10}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 20}, {"p1", false}, {"p2", true}});
    sequence.push_back(input_type{{"time", 30}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 40}});

    auto net1 =
      reelay::dense_timed_network<time_type, input_type>::make("{p1} -> {p2}");

    auto result = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result = result | net1.output();
    }

    auto expected = interval_set();
    expected.add(interval::left_open(0, 10));
    expected.add(interval::left_open(20, 40));

    CHECK(result == expected);
  }

  SECTION("Negation")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"p1", false}});
    sequence.push_back(input_type{{"time", 10}, {"p1", true}});
    sequence.push_back(input_type{{"time", 20}});

    auto net1 =
      reelay::dense_timed_network<time_type, input_type>::make("not{p1}");

    auto result = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result = result | net1.output();
    }

    auto expected = interval_set();
    expected.add(interval::left_open(0, 10));

    CHECK(result == expected);
  }
}

TEST_CASE(  // NOLINT(readability-function-cognitive-complexity)
  "Dense Timed Temporal Operations (Untimed)",
  "[dense_timed]")
{
  SECTION("Past Always")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 10}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 20}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 30}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 60}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 70}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 90}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 120}});

    auto net1 = reelay::dense_timed_network<time_type, input_type>::make(
      "historically{p1}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(0, 60));

    CHECK(result1 == expected1);
  }

  SECTION("Past Sometime")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 10}, {"p1", false}, {"p2", true}});
    sequence.push_back(input_type{{"time", 20}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 30}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 60}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 70}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 90}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 115}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 125}});

    auto net1 =
      reelay::dense_timed_network<time_type, input_type>::make("once{p1}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(60, 125));

    CHECK(result1 == expected1);
  }

  SECTION("Since")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 10}, {"p1", false}, {"p2", true}});
    sequence.push_back(input_type{{"time", 20}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 30}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 60}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 70}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 90}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 115}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 156}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 175}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 201}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 248}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 287}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 300}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 315}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 444}});

    auto net1 = reelay::dense_timed_network<time_type, input_type>::make(
      "{p1} since {p2}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(115, 201));

    CHECK(result1 == expected1);
  }
}

TEST_CASE(  // NOLINT(readability-function-cognitive-complexity)
  "Dense Timed Temporal Operations (Bounded)",
  "[dense_timed]")
{
  SECTION("Timed Past Always")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 10}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 20}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 30}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 60}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 70}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 90}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 115}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 156}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 175}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 201}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 248}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 287}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 300}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 315}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 444}});

    // auto net1 =
    // reelay::detail::dense_timed<time_type>::network<input_type>::make("not
    // (p1 since [12:24] not p2)");
    auto net1 = reelay::dense_timed_network<time_type, input_type>::make(
      "historically[12:24]{p2}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(0, 12));
    expected1.add(interval::left_open(139, 168));
    expected1.add(interval::left_open(199, 260));
    expected1.add(interval::left_open(311, 312));

    CHECK(result1 == expected1);
  }
  SECTION("Timed Past Always Zero")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 10}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 20}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 30}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 60}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 70}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 90}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 115}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 156}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 175}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 201}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 248}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 287}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 300}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 315}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 444}});

    // auto net1 =
    // reelay::detail::dense_timed<time_type>::network<input_type>::make("not
    // (p1 since [12:24] not p2)");
    auto net1 = reelay::dense_timed_network<time_type, input_type>::make(
      "historically[0:24]{p2}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(139, 156));
    expected1.add(interval::left_open(199, 248));

    CHECK(result1 == expected1);
  }

  SECTION("Timed Past Always Inf")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 10}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 20}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 30}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 60}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 70}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 90}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 115}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 156}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 175}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 201}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 248}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 287}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 300}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 315}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 444}});

    // auto net1 =
    // reelay::detail::dense_timed<time_type>::network<input_type>::make("not
    // (p1 since [12:24] not p2)");
    auto net1 = reelay::dense_timed_network<time_type, input_type>::make(
      "historically[12:]{p2}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(0, 12));

    CHECK(result1 == expected1);
  }

  SECTION("Timed Past Sometime")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 10}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 20}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 30}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 60}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 70}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 90}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 115}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 156}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 175}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 201}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 248}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 287}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 300}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 315}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 444}});

    auto net1 = reelay::dense_timed_network<time_type, input_type>::make(
      "once[12:24]{p2}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(22, 44));
    expected1.add(interval::left_open(127, 180));
    expected1.add(interval::left_open(187, 272));
    expected1.add(interval::left_open(299, 324));

    CHECK(result1 == expected1);
  }

  SECTION("Timed Past Sometime Zero")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 10}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 20}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 30}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 60}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 70}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 90}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 115}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 156}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 175}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 201}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 248}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 287}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 300}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 315}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 444}});
    // auto net1 =
    // reelay::detail::dense_timed<time_type>::network<input_type>::make("p1
    // since [12:24] p2");
    auto net1 =
      reelay::dense_timed_network<time_type, input_type>::make("once[:24]{p2}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(10, 44));
    expected1.add(interval::left_open(115, 180));
    expected1.add(interval::left_open(175, 272));
    expected1.add(interval::left_open(287, 324));

    CHECK(result1 == expected1);
  }

  SECTION("Timed Past Sometime Inf")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 10}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 20}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 30}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 60}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 70}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 90}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 115}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 156}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 175}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 201}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 248}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 287}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 300}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 315}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 444}});

    // auto net1 =
    // reelay::detail::dense_timed<time_type>::network<input_type>::make("p1
    // since [12:24] p2");
    auto net1 =
      reelay::dense_timed_network<time_type, input_type>::make("once[12:]{p2}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(22, 444));

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 10}, {"p1", false}, {"p2", true}});
    sequence.push_back(input_type{{"time", 20}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 30}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 60}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 70}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 90}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 115}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 156}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 175}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 201}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 248}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 287}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 300}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 315}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 444}});

    auto net1 = reelay::dense_timed_network<time_type, input_type>::make(
      "{p1} since[18:24]{p2}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set(interval::left_open(133, 180));

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since Zero")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 10}, {"p1", false}, {"p2", true}});
    sequence.push_back(input_type{{"time", 20}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 30}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 60}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 70}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 90}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 115}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 156}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 175}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 201}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 248}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 287}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 300}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 315}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 444}});

    auto net1 = reelay::dense_timed_network<time_type, input_type>::make(
      "{p1} since[:24]{p2}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set(interval::left_open(115, 180));

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 10}, {"p1", false}, {"p2", true}});
    sequence.push_back(input_type{{"time", 20}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 30}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 60}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 70}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 90}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 115}, {"p1", true}, {"p2", true}});
    sequence.push_back(input_type{{"time", 156}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 175}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 201}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 248}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 287}, {"p1", false}, {"p2", false}});
    sequence.push_back(input_type{{"time", 300}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 315}, {"p1", true}, {"p2", false}});
    sequence.push_back(input_type{{"time", 444}});

    auto net1 = reelay::dense_timed_network<time_type, input_type>::make(
      "{p1} since[18:]{p2}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set(interval::left_open(133, 201));

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since")
  {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"time", 0}, {"p1", false}});
    sequence.push_back(input_type{{"time", 10}, {"p1", false}});
    sequence.push_back(input_type{{"time", 20}, {"p1", false}});
    sequence.push_back(input_type{{"time", 30}, {"p1", true}});
    sequence.push_back(input_type{{"time", 60}, {"p1", true}});
    sequence.push_back(input_type{{"time", 70}, {"p1", true}});
    sequence.push_back(input_type{{"time", 90}, {"p1", true}});
    sequence.push_back(input_type{{"time", 115}, {"p1", false}});
    sequence.push_back(input_type{{"time", 156}, {"p1", true}});
    sequence.push_back(input_type{{"time", 175}, {"p1", true}});
    sequence.push_back(input_type{{"time", 201}, {"p1", false}});
    sequence.push_back(input_type{{"time", 248}, {"p1", false}});
    sequence.push_back(input_type{{"time", 287}, {"p1", false}});
    sequence.push_back(input_type{{"time", 300}, {"p1", true}});
    sequence.push_back(input_type{{"time", 315}, {"p1", true}});
    sequence.push_back(input_type{{"time", 444}});

    auto net1 = reelay::dense_timed_network<time_type, input_type>::make(
      "!{p1} since[10:20] {p1}");

    auto result1 = interval_set();

    for(const auto& s : sequence) {
      net1.update(s);
      result1 = result1 | net1.output();
    }

    auto expected1 = interval_set();
    expected1.add(interval::left_open(125, 135));
    expected1.add(interval::left_open(211, 221));

    CHECK(result1 == expected1);
  }
}

/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "iostream"
#include "limits"
#include "vector"

#include "catch.hpp"

#include "reelay/json.hpp"
#include "reelay/monitors.hpp"

using input_t = reelay::json;
using output_t = int64_t;
using function_t = std::function<output_t(const input_t &)>;

output_t top = std::numeric_limits<output_t>::max();
output_t bot = -std::numeric_limits<output_t>::max();

TEST_CASE("Atoms") {

  SECTION("Proposition") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 4}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 5}});

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "{x1}");

    auto result = std::vector<output_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<output_t>({3, 4, 4, 5});

    CHECK(result == expected);
  }

  SECTION("AtomicTrue") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", true}});
    sequence.push_back(input_t{{"x1", true}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", false}});

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "{x1: true}");

    auto result = std::vector<output_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<output_t>({top, top, top, bot});

    CHECK(result == expected);
  }

  SECTION("AtomicFalse") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", true}});
    sequence.push_back(input_t{{"x1", true}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", false}});

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "{x1: false}");

    auto result = std::vector<output_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<output_t>({bot, bot, bot, top});

    CHECK(result == expected);
  }

  SECTION("AtomicString") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", "a"}});
    sequence.push_back(input_t{{"x1", "b"}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", "c"}});

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "{x1: b}");

    auto result = std::vector<output_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<output_t>({bot, top, top, bot});

    CHECK(result == expected);
  }

  SECTION("AtomicNumber") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 1}});
    sequence.push_back(input_t{{"x1", 2}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 4}});

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "{x1: 2}");

    auto result = std::vector<output_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<output_t>({bot, top, top, bot});

    CHECK(result == expected);
  }

  SECTION("AtomicAny") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 1}});
    sequence.push_back(input_t{{"x2", 2}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 4}});

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "{x1: *}");

    auto result = std::vector<output_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<output_t>({top, bot, bot, top});

    CHECK(result == expected);
  }

  SECTION("GreaterThan") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 4}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 5}});

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "{x1 > 4}");

    auto result = std::vector<output_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<output_t>({-1, 0, 0, 1});

    CHECK(result == expected);
  }

  SECTION("GreaterEqual") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 4}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 5}});

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "{x1 >= 4}");

    auto result = std::vector<output_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<output_t>({-1, 0, 0, 1});

    CHECK(result == expected);
  }

  SECTION("LessThan") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 4}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 5}});

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "{x1 < 4}");

    auto result = std::vector<output_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<output_t>({1, 0, 0, -1});

    CHECK(result == expected);
  }

  SECTION("LessEqual") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 4}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 5}});

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "{x1 <= 4}");

    auto result = std::vector<output_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<output_t>({1, 0, 0, -1});

    CHECK(result == expected);
  }

  SECTION("Equal") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 4}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 5}});

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "{x1 == 4}");

    auto result = std::vector<output_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<output_t>({-1, 0, 0, -1});

    CHECK(result == expected);
  }

  SECTION("NotEqual") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 4}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 5}});

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "{x1 != 4}");

    auto result = std::vector<output_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<output_t>({1, 0, 0, 1});

    CHECK(result == expected);
  }

  SECTION("Record Double Comparison") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 2}});
    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 4}});
    sequence.push_back(input_t{{"x1", 5}});
    sequence.push_back(input_t{{"x1", 6}});

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "{x1 > 3, x1 < 5}");

    auto result = std::vector<output_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<output_t>({-1, 0, 1, 0, -1});

    CHECK(result == expected);
  }

  SECTION("CustomPredicates") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 3}, {"x2", 7}});
    sequence.push_back(input_t{{"x1", 4}, {"x2", 6}});
    sequence.push_back(input_t{{"x1", 5}, {"x2", 9}});

    function_t sum_x1_and_x2_gt_5 = [](const input_t &row) {
      return (row.at("x1").get<double>() + row.at("x2").get<double>()) - 11.0;
    };

    reelay::kwargs predicates = {{"sum_x1_and_x2_gt_5", sum_x1_and_x2_gt_5}};

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "${sum_x1_and_x2_gt_5}", predicates);

    auto result = std::vector<output_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<output_t>({-1, -1, 3});

    CHECK(result == expected);
  }
}

TEST_CASE("Nested Inputs") {

  SECTION("Deep Object") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"obj", {{"flag", 3}}}});
    sequence.push_back(input_t{{"obj", {{"flag", 4}}}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"obj", {{"flag", 5}}}});
    sequence.push_back(input_t{});

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "obj::{flag > 4}");

    auto result = std::vector<output_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<output_t>({-1, 0, 0, 1, 1});

    CHECK(result == expected);
  }

  SECTION("Deep Object 2") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(
        input_t{{"obj1", {{"obj2", {{"flag1", false}, {"flag2", 3}}}}}});
    sequence.push_back(
        input_t{{"obj1", {{"obj2", {{"flag1", false}, {"flag2", 4}}}}}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"obj1", {{"obj2", {{"flag2", 5}}}}}});
    sequence.push_back(input_t{});

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "obj1::obj2::{flag2 > 4}");

    auto result = std::vector<output_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<output_t>({-1, 0, 0, 1, 1});

    CHECK(result == expected);
  }

  // SECTION("Deep List Any") {
  //   std::vector<input_t> sequence = std::vector<input_t>();

  //   sequence.push_back(input_t{
  //       {"obj1", {{"obj2", {{{"a", 7}, {"b", 3}}, {{"a", 9}, {"b", 2}}}}}}});
  //   sequence.push_back(input_t{
  //       {"obj1", {{"obj2", {{{"a", 7}, {"b", 3}}, {{"a", 5}, {"b", 2}}}}}}});
  //   sequence.push_back(input_t{
  //       {"obj1", {{"obj2", {{{"a", 7}, {"b", 3}}, {{"a", 5}, {"b", 2}}}}}}});

  //   auto net1 =
  //       reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
  //           "obj1::obj2::any{a < 6}");

  //   auto result = std::vector<output_t>();

  //   for (const auto &row : sequence) {
  //     net1->update(row);
  //     result.push_back(net1->output());
  //   }

  //   auto expected = std::vector<output_t>({-1, 1, 1});

  //   CHECK(result == expected);
  // }

  // SECTION("Deep List All") {
  //   std::vector<input_t> sequence = std::vector<input_t>();

  //   sequence.push_back(input_t{
  //       {"obj1", {{"obj2", {{{"a", 7}, {"b", 3}}, {{"a", 9}, {"b", 2}}}}}}});
  //   sequence.push_back(input_t{
  //       {"obj1", {{"obj2", {{{"a", 7}, {"b", 3}}, {{"a", 5}, {"b", 2}}}}}}});
  //   sequence.push_back(input_t{
  //       {"obj1", {{"obj2", {{{"a", 7}, {"b", 3}}, {{"a", 5}, {"b", 0}}}}}}});

  //   auto net1 =
  //       reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
  //           "obj1::obj2::all{b > 1}");

  //   auto result = std::vector<output_t>();

  //   for (const auto &row : sequence) {
  //     net1->update(row);
  //     result.push_back(net1->output());
  //   }

  //   auto expected = std::vector<output_t>({1, 1, -1});

  //   CHECK(result == expected);
  // }
}

TEST_CASE("Boolean Operations") {

  SECTION("Disjunction") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 0}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 0}, {"p2", 3}});
    sequence.push_back(input_t{{"p1", 5}, {"p2", 4}});

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "{p1} or {p2}");

    auto result = std::vector<output_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<output_t>({0, 1, 3, 5});

    CHECK(result == expected);
  }

  SECTION("Conjuction") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 0}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 0}, {"p2", 3}});
    sequence.push_back(input_t{{"p1", 5}, {"p2", 4}});

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "{p1} and {p2}");

    auto result = std::vector<output_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<output_t>({0, 0, 0, 4});

    CHECK(result == expected);
  }

  SECTION("Implication") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 0}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 0}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 1}});

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "{p1} -> {p2}");

    auto result = std::vector<output_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<output_t>({0, 0, 1, 1});

    CHECK(result == expected);
  }

  SECTION("Negation") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", -10}});
    sequence.push_back(input_t{{"p1", 10}});

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "not{p1}");

    auto result = std::vector<output_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<output_t>({10, -10});

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

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "pre{p1}");
    auto net2 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "pre{p2}");

    auto result1 = std::vector<output_t>();
    auto result2 = std::vector<output_t>();

    for (const auto &s : sequence) {
      net1->update(s);
      net2->update(s);
      result1.push_back(net1->output());
      result2.push_back(net2->output());
    }

    auto expected1 = std::vector<output_t>({bot, 8, 7, 5});
    auto expected2 = std::vector<output_t>({bot, -1, 1, 0});

    CHECK(result1 == expected1);
    CHECK(result2 == expected2);
  }

  SECTION("Always") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 8}, {"p2", -1}});
    sequence.push_back(input_t{{"p1", 7}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", 5}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 6}, {"p2", 0}});

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "historically{p1}");
    auto net2 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "historically{p2}");

    auto result1 = std::vector<output_t>();
    auto result2 = std::vector<output_t>();

    for (const auto &s : sequence) {
      net1->update(s);
      net2->update(s);
      result1.push_back(net1->output());
      result2.push_back(net2->output());
    }

    auto expected1 = std::vector<output_t>({8, 7, 5, 5});
    auto expected2 = std::vector<output_t>({-1, -1, -1, -1});

    CHECK(result1 == expected1);
    CHECK(result2 == expected2);
  }

  SECTION("Once") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 0}, {"p2", 4}});
    sequence.push_back(input_t{{"p1", 6}, {"p2", 5}});
    sequence.push_back(input_t{{"p1", 7}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", 5}, {"p2", 8}});

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "once{p1}");
    auto net2 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "once{p2}");

    auto result1 = std::vector<output_t>();
    auto result2 = std::vector<output_t>();

    for (const auto &s : sequence) {
      net1->update(s);
      net2->update(s);
      result1.push_back(net1->output());
      result2.push_back(net2->output());
    }

    auto expected1 = std::vector<output_t>({0, 6, 7, 7});
    auto expected2 = std::vector<output_t>({4, 5, 5, 8});

    CHECK(result1 == expected1);
    CHECK(result2 == expected2);
  }

  SECTION("Since") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", 0}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 1}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 0}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 4}, {"p2", 1}});
    sequence.push_back(input_t{{"p1",-1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 0}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 0}, {"p2", 0}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 5}});
    sequence.push_back(input_t{{"p1", 1}, {"p2", 0}});

    auto net1 =
        reelay::robustness<output_t>::monitor<input_t>::from_temporal_logic(
            "{p1} since {p2}");

    auto result1 = std::vector<output_t>();

    for (const auto &s : sequence) {
      net1->update(s);
      result1.push_back(net1->output());
    }

    auto expected1 =
        std::vector<output_t>({0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 5, 1});

    CHECK(result1 == expected1);
  }
}

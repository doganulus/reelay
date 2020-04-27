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
#include "reelay/json.hpp"
#include "reelay/networks.hpp"
#include "vector"

using input_t = reelay::json;
using function_t = std::function<bool(const input_t &)>;

TEST_CASE("Atoms") {
  SECTION("AtomicProposition") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", false}});
    sequence.push_back(input_t{{"x1", false}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", true}});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("{x1}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({0, 0, 0, 1});

    CHECK(result == expected);
  }

  SECTION("AtomicTrue") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", false}});
    sequence.push_back(input_t{{"x1", false}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", true}});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("{x1:true}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({0, 0, 0, 1});

    CHECK(result == expected);
  }

  SECTION("AtomicFalse") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", false}});
    sequence.push_back(input_t{{"x1", false}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", true}});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("{x1:false}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({1, 1, 1, 0});

    CHECK(result == expected);
  }

  SECTION("GreaterThan") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 4}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 5}});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("{x1 > 4}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({0, 0, 0, 1});

    CHECK(result == expected);
  }

  SECTION("GreaterEqual") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 4}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 5}});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("{x1 >= 4}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({0, 1, 1, 1});

    CHECK(result == expected);
  }

  SECTION("LessThan") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 4}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 5}});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("{x1 < 4}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({1, 0, 0, 0});

    CHECK(result == expected);
  }

  SECTION("LessEqual") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 4}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 5}});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("{x1 <= 4}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({1, 1, 1, 0});

    CHECK(result == expected);
  }

  SECTION("AtomicNotEqual") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 4}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 5}});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("{x1 != 4}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({1, 0, 0, 1});

    CHECK(result == expected);
  }

  SECTION("AtomicSimpleString") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", "a"}});
    sequence.push_back(input_t{{"x1", "b"}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", "c"}});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("{x1: a}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({1, 0, 0, 0});

    CHECK(result == expected);
  }
  SECTION("AtomicNumber") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 1}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x1", 3}});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("{x1: 3}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({1, 0, 0, 1});

    CHECK(result == expected);
  }

  SECTION("AtomicAny") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", "3"}, {"x2", "a"}});
    sequence.push_back(input_t{{"x1", true}});
    sequence.push_back(input_t{{"x1", "3"}, {"x2", "c"}});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("{x2: *}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({1, 0, 1});

    CHECK(result == expected);
  }

  SECTION("CustomPredicates") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 3.3}, {"x2", 1.7}});
    sequence.push_back(input_t{{"x1", 3.4}, {"x2", 1.6}});
    sequence.push_back(input_t{{"x1", 3.5}, {"x2", 1.9}});

    function_t sum_x1_and_x2_gt_5 = [](const input_t &row) {
      return (row.at("x1").get<double>() + row.at("x2").get<double>()) > 5.0;
    };

    reelay::kwargs predicates = {{"sum_x1_and_x2_gt_5", sum_x1_and_x2_gt_5}};

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("${sum_x1_and_x2_gt_5}", predicates);

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({0, 0, 1});

    CHECK(result == expected);
  }

  SECTION("Record Double Comparison") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", 2}});
    sequence.push_back(input_t{{"x1", 3}});
    sequence.push_back(input_t{{"x1", 4}});
    sequence.push_back(input_t{{"x1", 5}});
    sequence.push_back(input_t{{"x1", 6}});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("{x1 > 3, x1 < 5}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({0, 0, 1, 0, 0});

    CHECK(result == expected);
  }
}

TEST_CASE("Nested Inputs") {
  SECTION("Deep Object") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"obj", {{"flag", false}}}});
    sequence.push_back(input_t{{"obj", {{"flag", false}}}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"obj", {{"flag", true}}}});
    sequence.push_back(input_t{});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("obj::{flag}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({0, 0, 0, 1, 1});

    CHECK(result == expected);
  }

  SECTION("Deep Object 2") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(
        input_t{{"obj1", {{"obj2", {{"flag1", false}, {"flag2", false}}}}}});
    sequence.push_back(
        input_t{{"obj1", {{"obj2", {{"flag1", false}, {"flag2", true}}}}}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"obj1", {{"obj2", {{"flag2", false}}}}}});
    sequence.push_back(input_t{});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("obj1::obj2::{flag2}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({0, 1, 1, 0, 0});

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
  //       reelay::detail::discrete_timed<int>::network<input_t>::from_temporal_logic(
  //           "obj1::obj2::any{a < 6}");

  //   auto result = std::vector<bool>();

  //   for (const auto &row : sequence) {
  //     net1->update(row);
  //     result.push_back(net1->output());
  //   }

  //   auto expected = std::vector<bool>({0, 1, 1});

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
  //       reelay::detail::discrete_timed<int>::network<input_t>::from_temporal_logic(
  //           "obj1::obj2::all{b > 1}");

  //   auto result = std::vector<bool>();

  //   for (const auto &row : sequence) {
  //     net1->update(row);
  //     result.push_back(net1->output());
  //   }

  //   auto expected = std::vector<bool>({1, 1, 0});

  //   CHECK(result == expected);
  // }
}

TEST_CASE("Boolean Operations") {
  SECTION("Disjunction") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", false}, {"p2", true}});
    sequence.push_back(input_t{{"p1", true}, {"p2", true}});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("{p1} or {p2}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({0, 1, 1, 1});

    CHECK(result == expected);
  }

  SECTION("Conjuction") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", false}, {"p2", true}});
    sequence.push_back(input_t{{"p1", true}, {"p2", true}});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("{p1} and {p2}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({0, 0, 0, 1});

    CHECK(result == expected);
  }

  SECTION("Implication") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", false}, {"p2", true}});
    sequence.push_back(input_t{{"p1", true}, {"p2", true}});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("{p1} -> {p2}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({1, 0, 1, 1});

    CHECK(result == expected);
  }

  SECTION("Negation") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", false}});
    sequence.push_back(input_t{{"p1", true}});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("not {p1}");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({1, 0});

    CHECK(result == expected);
  }
}

TEST_CASE("Untimed Temporal Operations") {
  SECTION("Previous") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", false}, {"p2", true}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", false}, {"p2", true}});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("pre{p1}");
    auto net2 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("pre{p2}");

    auto result1 = std::vector<bool>();
    auto result2 = std::vector<bool>();

    for (const auto &s : sequence) {
      net1->update(s);
      net2->update(s);
      result1.push_back(net1->output());
      result2.push_back(net2->output());
    }

    auto expected1 = std::vector<bool>({0, 1, 0, 1});
    auto expected2 = std::vector<bool>({0, 0, 1, 0});

    CHECK(result1 == expected1);
    CHECK(result2 == expected2);
  }

  SECTION("Past Always") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", true}, {"p2", true}});
    sequence.push_back(input_t{{"p1", true}, {"p2", true}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("historically{p1}");
    auto net2 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("historically{p2}");

    auto result1 = std::vector<bool>();
    auto result2 = std::vector<bool>();

    for (const auto &s : sequence) {
      net1->update(s);
      net2->update(s);
      result1.push_back(net1->output());
      result2.push_back(net2->output());
    }

    auto expected1 = std::vector<bool>({1, 1, 1, 1});
    auto expected2 = std::vector<bool>({1, 1, 0, 0});

    CHECK(result1 == expected1);
    CHECK(result2 == expected2);
  }

  SECTION("Past Sometime") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", false}, {"p2", false}});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("once{p1}");
    auto net2 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("once{p2}");

    auto result1 = std::vector<bool>();
    auto result2 = std::vector<bool>();

    for (const auto &s : sequence) {
      net1->update(s);
      net2->update(s);
      result1.push_back(net1->output());
      result2.push_back(net2->output());
    }

    auto expected1 = std::vector<bool>({0, 0, 1, 1});
    auto expected2 = std::vector<bool>({0, 0, 0, 0});

    CHECK(result1 == expected1);
    CHECK(result2 == expected2);
  }

  SECTION("Since") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", true}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", true}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", true}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("{p1} since {p2}");

    auto result1 = std::vector<bool>();

    for (const auto &s : sequence) {
      net1->update(s);
      result1.push_back(net1->output());
    }

    auto expected1 =
        std::vector<bool>({0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1});

    CHECK(result1 == expected1);
  }
}

TEST_CASE("Timed Temporal Operations") {
  SECTION("Timed Once") {
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

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("once[2:4]{p2}");

    auto result1 = std::vector<bool>();

    for (const auto &s : sequence) {
      net1->update(s);
      result1.push_back(net1->output());
    }

    auto expected1 = std::vector<bool>({0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Once Zero") {
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

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("once[:4]{p2}");

    auto result1 = std::vector<bool>();

    for (const auto &s : sequence) {
      net1->update(s);
      result1.push_back(net1->output());
    }

    auto expected1 = std::vector<bool>({0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Once Inf") {
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

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("once[2:]{p2}");

    auto result1 = std::vector<bool>();

    for (const auto &s : sequence) {
      net1->update(s);
      result1.push_back(net1->output());
    }

    auto expected1 = std::vector<bool>({0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Always") {
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

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("historically[2:4]{p1}");

    auto result1 = std::vector<bool>();

    for (const auto &s : sequence) {
      net1->update(s);
      result1.push_back(net1->output());
    }

    auto expected1 = std::vector<bool>({1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Always Zero") {
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

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("historically[:4]{p1}");

    auto result1 = std::vector<bool>();

    for (const auto &s : sequence) {
      net1->update(s);
      result1.push_back(net1->output());
    }

    auto expected1 = std::vector<bool>({0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Always Inf") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", true}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", false}});
    sequence.push_back(input_t{{"p1", true}, {"p2", true}});
    sequence.push_back(input_t{{"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"p1", false}, {"p2", false}});
    sequence.push_back(input_t{{"p1", false}, {"p2", false}});

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("historically[2:]{p1}");

    auto result1 = std::vector<bool>();

    for (const auto &s : sequence) {
      net1->update(s);
      result1.push_back(net1->output());
    }

    auto expected1 = std::vector<bool>({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since") {
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

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("{p1} since[2:4] {p2}");

    auto result1 = std::vector<bool>();

    for (const auto &s : sequence) {
      net1->update(s);
      result1.push_back(net1->output());
    }

    auto expected1 = std::vector<bool>({0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since Zero") {
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

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("{p1} since[:4] {p2}");

    auto result1 = std::vector<bool>();

    for (const auto &s : sequence) {
      net1->update(s);
      result1.push_back(net1->output());
    }

    auto expected1 = std::vector<bool>({0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0});

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since Inf") {
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

    auto net1 = reelay::detail::discrete_timed<int>::network<
        input_t>::from_temporal_logic("{p1} since[2:] {p2}");

    auto result1 = std::vector<bool>();

    for (const auto &s : sequence) {
      net1->update(s);
      result1.push_back(net1->output());
    }

    auto expected1 = std::vector<bool>({0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0});

    CHECK(result1 == expected1);
  }
}

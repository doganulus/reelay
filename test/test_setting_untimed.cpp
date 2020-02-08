/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "iostream"
#include "vector"

#include "catch.hpp"

#include "reelay/common.hpp"
#include "reelay/monitors.hpp"

using input_t = std::map<std::string, std::string>;
using function_t = std::function<bool(const input_t &)>;

TEST_CASE("Atoms") {

  SECTION("GreaterThan") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", "3.3"}});
    sequence.push_back(input_t{{"x1", "3.4"}});
    sequence.push_back(input_t{{"x1", "3.5"}});

    auto net1 = reelay::monitor<input_t>::from_temporal_logic("x1 > 3.4");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({0, 0, 1});

    CHECK(result == expected);
  }

  SECTION("GreaterEqual") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", "3.3"}});
    sequence.push_back(input_t{{"x1", "3.4"}});
    sequence.push_back(input_t{{"x1", "3.5"}});

    auto net1 = reelay::monitor<input_t>::from_temporal_logic("x1 >= 3.4");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({0, 1, 1});

    CHECK(result == expected);
  }

  SECTION("LessThan") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", "3.3"}});
    sequence.push_back(input_t{{"x1", "3.4"}});
    sequence.push_back(input_t{{"x1", "3.5"}});

    auto net1 = reelay::monitor<input_t>::from_temporal_logic("x1 < 3.4");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({1, 0, 0});

    CHECK(result == expected);
  }

  SECTION("LessEqual") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", "3.3"}});
    sequence.push_back(input_t{{"x1", "3.4"}});
    sequence.push_back(input_t{{"x1", "3.5"}});

    auto net1 = reelay::monitor<input_t>::from_temporal_logic("x1 <= 3.4");

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({1, 1, 0});

    CHECK(result == expected);
  }

  SECTION("CustomPredicates") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x1", "3.3"}, {"x2", "1.7"}});
    sequence.push_back(input_t{{"x1", "3.4"}, {"x2", "1.6"}});
    sequence.push_back(input_t{{"x1", "3.5"}, {"x2", "1.9"}});

    function_t sum_x1_and_x2_gt_5 = [](const input_t &row) {
      return (std::stof(row.at("x1")) + std::stof(row.at("x2"))) > 5.0;
    };

    reelay::kwargs predicates = {{"sum_x1_and_x2_gt_5", sum_x1_and_x2_gt_5}};

    auto net1 = reelay::monitor<input_t>::from_temporal_logic(
        "$sum_x1_and_x2_gt_5", predicates);

    auto result = std::vector<bool>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto expected = std::vector<bool>({0, 0, 1});

    CHECK(result == expected);
  }
}

TEST_CASE("Boolean Operations") {

  SECTION("Disjunction") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", "0"}, {"p2", "0"}});
    sequence.push_back(input_t{{"p1", "1"}, {"p2", "0"}});
    sequence.push_back(input_t{{"p1", "0"}, {"p2", "1"}});
    sequence.push_back(input_t{{"p1", "1"}, {"p2", "1"}});

    auto net1 = reelay::monitor<input_t>::from_temporal_logic("p1 or p2");

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

    sequence.push_back(input_t{{"p1", "0"}, {"p2", "0"}});
    sequence.push_back(input_t{{"p1", "1"}, {"p2", "0"}});
    sequence.push_back(input_t{{"p1", "0"}, {"p2", "1"}});
    sequence.push_back(input_t{{"p1", "1"}, {"p2", "1"}});

    auto net1 = reelay::monitor<input_t>::from_temporal_logic("p1 and p2");

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

    sequence.push_back(input_t{{"p1", "0"}, {"p2", "0"}});
    sequence.push_back(input_t{{"p1", "1"}, {"p2", "0"}});
    sequence.push_back(input_t{{"p1", "0"}, {"p2", "1"}});
    sequence.push_back(input_t{{"p1", "1"}, {"p2", "1"}});

    auto net1 = reelay::monitor<input_t>::from_temporal_logic("p1 -> p2");

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

    sequence.push_back(input_t{{"p1", "0"}});
    sequence.push_back(input_t{{"p1", "1"}});

    auto net1 = reelay::monitor<input_t>::from_temporal_logic("not p1");

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

    sequence.push_back(input_t{{"p1", "1"}, {"p2", "0"}});
    sequence.push_back(input_t{{"p1", "0"}, {"p2", "1"}});
    sequence.push_back(input_t{{"p1", "1"}, {"p2", "0"}});
    sequence.push_back(input_t{{"p1", "0"}, {"p2", "1"}});

    auto net1 = reelay::monitor<input_t>::from_temporal_logic("pre p1");
    auto net2 = reelay::monitor<input_t>::from_temporal_logic("pre p2");

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

  SECTION("Always") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", "1"}, {"p2", "1"}});
    sequence.push_back(input_t{{"p1", "1"}, {"p2", "1"}});
    sequence.push_back(input_t{{"p1", "1"}, {"p2", "0"}});
    sequence.push_back(input_t{{"p1", "1"}, {"p2", "0"}});

    auto net1 =
        reelay::monitor<input_t>::from_temporal_logic("historically p1");
    auto net2 =
        reelay::monitor<input_t>::from_temporal_logic("historically p2");

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

  SECTION("Once") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"p1", "0"}, {"p2", "0"}});
    sequence.push_back(input_t{{"p1", "0"}, {"p2", "0"}});
    sequence.push_back(input_t{{"p1", "1"}, {"p2", "0"}});
    sequence.push_back(input_t{{"p1", "0"}, {"p2", "0"}});

    auto net1 = reelay::monitor<input_t>::from_temporal_logic("once p1");
    auto net2 = reelay::monitor<input_t>::from_temporal_logic("once p2");

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

    sequence.push_back(input_t{{"p1", "0"}, {"p2", "0"}});
    sequence.push_back(input_t{{"p1", "1"}, {"p2", "1"}});
    sequence.push_back(input_t{{"p1", "1"}, {"p2", "0"}});
    sequence.push_back(input_t{{"p1", "1"}, {"p2", "0"}});
    sequence.push_back(input_t{{"p1", "1"}, {"p2", "0"}});
    sequence.push_back(input_t{{"p1", "0"}, {"p2", "0"}});
    sequence.push_back(input_t{{"p1", "1"}, {"p2", "0"}});
    sequence.push_back(input_t{{"p1", "1"}, {"p2", "1"}});
    sequence.push_back(input_t{{"p1", "1"}, {"p2", "0"}});
    sequence.push_back(input_t{{"p1", "1"}, {"p2", "0"}});
    sequence.push_back(input_t{{"p1", "0"}, {"p2", "0"}});
    sequence.push_back(input_t{{"p1", "0"}, {"p2", "0"}});
    sequence.push_back(input_t{{"p1", "1"}, {"p2", "1"}});
    sequence.push_back(input_t{{"p1", "1"}, {"p2", "0"}});

    auto net1 = reelay::monitor<input_t>::from_temporal_logic("p1 since p2");

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

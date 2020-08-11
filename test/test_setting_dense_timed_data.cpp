/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <iostream>
#include <vector>

#include "catch.hpp"
//
#include "reelay/common.hpp"
#include "reelay/intervals.hpp"
#include "reelay/json.hpp"
//
#include "reelay/networks/dense_timed_data_network.hpp"
#include "reelay/options.hpp"

using input_t = reelay::json;
using time_type = int64_t;

using interval = reelay::interval<time_type>;
using interval_map = reelay::data_interval_map<time_type>;

TEST_CASE("Atoms") {
  auto manager = std::make_shared<reelay::binding_manager>();
  auto opts = reelay::basic_options().with_data_manager(manager);

  SECTION("Proposition_ZeroHoldForward") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x1", true}});
    sequence.push_back(input_t{{"time", 31}});
    sequence.push_back(input_t{{"time", 33}, {"x1", false}});
    sequence.push_back(input_t{{"time", 35}, {"x1", false}});
    sequence.push_back(input_t{{"time", 40}});
    sequence.push_back(input_t{{"time", 52}, {"x1", true}});
    sequence.push_back(input_t{{"time", 55}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "{x1}", opts);

    auto result1 = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result1.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected1 = interval_map();
    expected1.add(std::make_pair(interval::left_open(0, 33), t));
    expected1.add(std::make_pair(interval::left_open(33, 52), f));
    expected1.add(std::make_pair(interval::left_open(52, 55), t));

    CHECK(result1 == expected1);
  }

  SECTION("AtomicAny_ZeroHoldForward") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x1", true}});
    sequence.push_back(input_t{{"time", 31}});
    sequence.push_back(input_t{{"time", 33}, {"x1", false}});
    sequence.push_back(input_t{{"time", 35}, {"x1", false}});
    sequence.push_back(input_t{{"time", 40}});
    sequence.push_back(input_t{{"time", 52}, {"x1", true}});
    sequence.push_back(input_t{{"time", 55}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "{x1: *}", opts);

    auto result1 = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result1.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected1 = interval_map();
    expected1.add(std::make_pair(interval::left_open(0, 31), t));
    expected1.add(std::make_pair(interval::left_open(31, 33), f));
    expected1.add(std::make_pair(interval::left_open(33, 40), t));
    expected1.add(std::make_pair(interval::left_open(40, 52), f));
    expected1.add(std::make_pair(interval::left_open(52, 55), t));

    CHECK(result1 == expected1);
  }

  SECTION("BooleanTrue_ZeroHoldForward") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x1", true}});
    sequence.push_back(input_t{{"time", 31}});
    sequence.push_back(input_t{{"time", 33}, {"x1", false}});
    sequence.push_back(input_t{{"time", 35}, {"x1", false}});
    sequence.push_back(input_t{{"time", 40}});
    sequence.push_back(input_t{{"time", 52}, {"x1", true}});
    sequence.push_back(input_t{{"time", 55}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "{x1: true}", opts);

    auto result1 = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result1.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected1 = interval_map();
    expected1.add(std::make_pair(interval::left_open(0, 33), t));
    expected1.add(std::make_pair(interval::left_open(33, 52), f));
    expected1.add(std::make_pair(interval::left_open(52, 55), t));

    CHECK(result1 == expected1);
  }

  SECTION("BooleanFalse_ZeroHoldForward") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x1", true}});
    sequence.push_back(input_t{{"time", 31}});
    sequence.push_back(input_t{{"time", 33}, {"x1", false}});
    sequence.push_back(input_t{{"time", 35}, {"x1", false}});
    sequence.push_back(input_t{{"time", 40}});
    sequence.push_back(input_t{{"time", 52}, {"x1", true}});
    sequence.push_back(input_t{{"time", 55}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "{x1: false}", opts);

    auto result1 = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result1.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected1 = interval_map();
    expected1.add(std::make_pair(interval::left_open(0, 33), f));
    expected1.add(std::make_pair(interval::left_open(33, 52), t));
    expected1.add(std::make_pair(interval::left_open(52, 55), f));

    CHECK(result1 == expected1);
  }

  SECTION("AtomicString_ZeroHoldForward") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x1", "veritas"}});
    sequence.push_back(input_t{{"time", 31}});
    sequence.push_back(input_t{{"time", 33}, {"x1", "fortuna"}});
    sequence.push_back(input_t{{"time", 35}, {"x1", "fortuna"}});
    sequence.push_back(input_t{{"time", 40}});
    sequence.push_back(input_t{{"time", 52}, {"x1", "veritas"}});
    sequence.push_back(input_t{{"time", 55}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "{x1: veritas}", opts);

    auto result1 = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result1.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected1 = interval_map();
    expected1.add(std::make_pair(interval::left_open(0, 33), t));
    expected1.add(std::make_pair(interval::left_open(33, 52), f));
    expected1.add(std::make_pair(interval::left_open(52, 55), t));

    CHECK(result1 == expected1);
  }

  SECTION("AtomicNumber_ZeroHoldForward") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x1", 12}});
    sequence.push_back(input_t{{"time", 31}});
    sequence.push_back(input_t{{"time", 33}, {"x1", 11}});
    sequence.push_back(input_t{{"time", 35}, {"x1", 12.5}});
    sequence.push_back(input_t{{"time", 40}});
    sequence.push_back(input_t{{"time", 52}, {"x1", 12.5}});
    sequence.push_back(input_t{{"time", 55}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "{x1: 12.5}", opts);

    auto result1 = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result1.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected1 = interval_map();
    expected1.add(std::make_pair(interval::left_open(0, 35), f));
    expected1.add(std::make_pair(interval::left_open(35, 55), t));

    CHECK(result1 == expected1);
  }

  SECTION("LessThan_ZeroHoldForward") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x1", 3}});
    sequence.push_back(input_t{{"time", 31}, {"x1", 3}});
    sequence.push_back(input_t{{"time", 33}, {"x1", 5}});
    sequence.push_back(input_t{{"time", 35}, {"x1", 4}});
    sequence.push_back(input_t{{"time", 40}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "{x1 < 4}", opts);

    auto result1 = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result1.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected1 = interval_map();
    expected1.add(std::make_pair(interval::left_open(0, 33), t));
    expected1.add(std::make_pair(interval::left_open(33, 40), f));

    CHECK(result1 == expected1);
  }

  SECTION("LessEqual_ZeroHoldForward") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x1", 3}});
    sequence.push_back(input_t{{"time", 31}, {"x1", 3}});
    sequence.push_back(input_t{{"time", 33}, {"x1", 5}});
    sequence.push_back(input_t{{"time", 35}, {"x1", 4}});
    sequence.push_back(input_t{{"time", 40}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "{x1 <= 4}", opts);

    auto result1 = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result1.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected1 = interval_map();
    expected1.add(std::make_pair(interval::left_open(0, 33), t));
    expected1.add(std::make_pair(interval::left_open(33, 35), f));
    expected1.add(std::make_pair(interval::left_open(35, 40), t));

    CHECK(result1 == expected1);
  }

  SECTION("GreaterThan_ZeroHoldForward") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x1", 3}});
    sequence.push_back(input_t{{"time", 31}, {"x1", 3}});
    sequence.push_back(input_t{{"time", 33}, {"x1", 5}});
    sequence.push_back(input_t{{"time", 35}, {"x1", 4}});
    sequence.push_back(input_t{{"time", 40}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "{x1 > 4}", opts);

    auto result1 = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result1.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected1 = interval_map();
    expected1.add(std::make_pair(interval::left_open(0, 33), f));
    expected1.add(std::make_pair(interval::left_open(33, 35), t));
    expected1.add(std::make_pair(interval::left_open(35, 40), f));

    CHECK(result1 == expected1);
  }

  SECTION("GreaterEqual_ZeroHoldForward") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x1", 3}});
    sequence.push_back(input_t{{"time", 31}, {"x1", 3}});
    sequence.push_back(input_t{{"time", 33}, {"x1", 5}});
    sequence.push_back(input_t{{"time", 35}, {"x1", 4}});
    sequence.push_back(input_t{{"time", 40}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "{x1 >= 4}", opts);

    auto result1 = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result1.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected1 = interval_map();
    expected1.add(std::make_pair(interval::left_open(0, 33), f));
    expected1.add(std::make_pair(interval::left_open(33, 40), t));

    CHECK(result1 == expected1);
  }

  SECTION("Record Proposition 0") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{
        {"time", 0},
        {"event", "access"},
        {"user", "alice"},
        {"file", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 3},
        {"event", "access"},
        {"user", "alice"},
        {"file", "feed_your_head"}});
    sequence.push_back(input_t{
        {"time", 6},
        {"event", "access"},
        {"user", "alice"},
        {"file", "wonderland"},
        {"dummy_field", "v2"}});
    sequence.push_back(input_t{{"time", 9}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "{event: access, user: alice, file: wonderland}", opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 3), t));
    expected.add(std::make_pair(interval::left_open(3, 6), f));
    expected.add(std::make_pair(interval::left_open(6, 9), t));

    CHECK(result == expected);
  }

  SECTION("Record Proposition 1") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{
        {"time", 0},
        {"event", "access"},
        {"user", "alice"},
        {"file", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 3},
        {"event", "access"},
        {"user", "bob"},
        {"file", "feed_your_head"}});
    sequence.push_back(input_t{
        {"time", 6},
        {"event", "access"},
        {"user", "charlotte"},
        {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", 9}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "{event: access, user: *name, file: wonderland}", opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto d1 = manager->assign("name", "alice");
    auto d3 = manager->assign("name", "charlotte");

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 3), d1));
    expected.add(std::make_pair(interval::left_open(3, 6), f));
    expected.add(std::make_pair(interval::left_open(6, 9), d3));

    CHECK(result == expected);
  }

  SECTION("Record Proposition 2") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{
        {"time", 0},
        {"event", "access"},
        {"user", "alice"},
        {"file", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 3},
        {"event", "access"},
        {"user", "bob"},
        {"file", "feed_your_head"}});
    sequence.push_back(input_t{
        {"time", 6},
        {"event", "access"},
        {"user", "charlotte"},
        {"file", "wonderland"}});
    sequence.push_back(
        input_t{{"time", 9}, {"event", "access"}, {"user", "charlotte"}});
    sequence.push_back(input_t{{"time", 11}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "{event: access, user: *name, file:*}", opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto d1 = manager->assign("name", "alice");
    auto d2 = manager->assign("name", "bob");
    auto d3 = manager->assign("name", "charlotte");

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 3), d1));
    expected.add(std::make_pair(interval::left_open(3, 6), d2));
    expected.add(std::make_pair(interval::left_open(6, 9), d3));
    expected.add(std::make_pair(interval::left_open(9, 11), f));

    CHECK(result == expected);
  }

  SECTION("Record Proposition 2") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(
        input_t{{"time", 0}, {"event", "access"}, {"user", "alice"}});
    sequence.push_back(
        input_t{{"time", 3}, {"event", "access"}, {"user", "bob"}});
    sequence.push_back(input_t{
        {"time", 6},
        {"event", "access"},
        {"user", "charlotte"},
        {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", 9}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "{file: wonderland}", opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 6), f));
    expected.add(std::make_pair(interval::left_open(6, 9), t));

    CHECK(result == expected);
  }

  SECTION("Record Proposition") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(
        input_t{{"time", 0}, {"event", "access"}, {"user", "alice"}});
    sequence.push_back(
        input_t{{"time", 3}, {"event", "access"}, {"file", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 6},
        {"event", "access"},
        {"user", "charlotte"},
        {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", 9}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "{file: wonderland, user:*}", opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 6), f));
    expected.add(std::make_pair(interval::left_open(6, 9), t));

    CHECK(result == expected);
  }
}

TEST_CASE("Boolean Operations") {
  auto manager = std::make_shared<reelay::binding_manager>();
  auto opts = reelay::basic_options().with_data_manager(manager);
  SECTION("Negation") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{
        {"time", 0},
        {"event", "access"},
        {"user", "alice"},
        {"file", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 1},
        {"event", "access"},
        {"user", "alice"},
        {"file", "feed_your_head"}});
    sequence.push_back(input_t{
        {"time", 3},
        {"event", "access"},
        {"user", "charlotte"},
        {"file", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 6},
        {"event", "logout"},
        {"user", "alice"},
        {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", 10}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "!{event: access, user: alice}", opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 3), f));
    expected.add(std::make_pair(interval::left_open(3, 10), t));

    CHECK(result == expected);
  }

  SECTION("Conjunction") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{
        {"time", 0},
        {"event", "access"},
        {"user", "alice"},
        {"file", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 1},
        {"event", "access"},
        {"user", "alice"},
        {"file", "feed_your_head"}});
    sequence.push_back(input_t{
        {"time", 3},
        {"event", "access"},
        {"user", "charlotte"},
        {"file", "wonderland"}});
    sequence.push_back(
        input_t{{"time", 6}, {"event", "logout"}, {"user", "alice"}});
    sequence.push_back(input_t{{"time", 10}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "{event: *e,     user: alice, file: *f} and"
        "{event: access, user: *u,    file: wonderland}",
        opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto alice_in_wland = manager->assign("e", "access")
                          * manager->assign("u", "alice")
                          * manager->assign("f", "wonderland");

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 1), alice_in_wland));
    expected.add(std::make_pair(interval::left_open(1, 10), f));

    CHECK(result == expected);
  }

  SECTION("Disjunction") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{
        {"time", 0},
        {"event", "access"},
        {"user", "alice"},
        {"file", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 1},
        {"event", "access"},
        {"user", "alice"},
        {"file", "feed_your_head"}});
    sequence.push_back(input_t{
        {"time", 3},
        {"event", "access"},
        {"user", "charlotte"},
        {"file", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 6},
        {"event", "access"},
        {"user", "square"},
        {"file", "flatland"}});
    sequence.push_back(input_t{{"time", 10}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "{event: *e,     user: alice, file: *f} or"
        "{event: access, user: *u,    file: wonderland}",
        opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto datum1
        = (manager->assign("e", "access") * manager->assign("f", "wonderland"))
          + manager->assign("u", "alice");
    auto datum2 = manager->assign("e", "access")
                  * manager->assign("f", "feed_your_head");
    auto datum3 = manager->assign("u", "charlotte");

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 1), datum1));
    expected.add(std::make_pair(interval::left_open(1, 3), datum2));
    expected.add(std::make_pair(interval::left_open(3, 6), datum3));
    expected.add(std::make_pair(interval::left_open(6, 10), f));

    CHECK(result == expected);
  }

  SECTION("Implication") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{
        {"time", 0},
        {"event", "access"},
        {"user", "alice"},
        {"file", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 1},
        {"event", "access"},
        {"user", "alice"},
        {"file", "feed_your_head"}});
    sequence.push_back(input_t{
        {"time", 3},
        {"event", "access"},
        {"user", "charlotte"},
        {"file", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 6},
        {"event", "access"},
        {"user", "square"},
        {"file", "flatland"}});
    sequence.push_back(input_t{{"time", 10}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "not{event: *e, user: alice, file: *f} implies"
        "{event: access, user: *u, file: wonderland}",
        opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto datum1
        = (manager->assign("e", "access") * manager->assign("f", "wonderland"))
          + manager->assign("u", "alice");
    auto datum2 = manager->assign("e", "access")
                  * manager->assign("f", "feed_your_head");
    auto datum3 = manager->assign("u", "charlotte");

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 1), datum1));
    expected.add(std::make_pair(interval::left_open(1, 3), datum2));
    expected.add(std::make_pair(interval::left_open(3, 6), datum3));
    expected.add(std::make_pair(interval::left_open(6, 10), f));

    CHECK(result == expected);
  }

  SECTION("Existential Quantification") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{
        {"time", 0},
        {"event", "access"},
        {"user", "alice"},
        {"file", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 1},
        {"event", "access"},
        {"user", "alice"},
        {"file", "feed_your_head"}});
    sequence.push_back(input_t{
        {"time", 3},
        {"event", "access"},
        {"user", "charlotte"},
        {"file", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 6},
        {"event", "access"},
        {"user", "square"},
        {"file", "flatland"}});
    sequence.push_back(input_t{{"time", 10}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "exists[e]. {event: *e, user: alice, file: *f}", opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto datum1 = manager->assign("f", "wonderland");
    auto datum2 = manager->assign("f", "feed_your_head");

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 1), datum1));
    expected.add(std::make_pair(interval::left_open(1, 3), datum2));
    expected.add(std::make_pair(interval::left_open(3, 10), f));

    CHECK(result == expected);
  }

  SECTION("Existential Quantification") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{
        {"time", 0},
        {"event", "access"},
        {"user", "alice"},
        {"file", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 1},
        {"event", "access"},
        {"user", "alice"},
        {"file", "feed_your_head"}});
    sequence.push_back(input_t{
        {"time", 3},
        {"event", "access"},
        {"user", "charlotte"},
        {"file", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 6},
        {"event", "access"},
        {"user", "square"},
        {"file", "flatland"}});
    sequence.push_back(input_t{{"time", 10}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "not(forall[e].(not{event: *e, user: alice, file: *f}))", opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto datum1 = manager->assign("f", "wonderland");
    auto datum2 = manager->assign("f", "feed_your_head");

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 1), datum1));
    expected.add(std::make_pair(interval::left_open(1, 3), datum2));
    expected.add(std::make_pair(interval::left_open(3, 10), f));

    CHECK(result == expected);
  }
}

TEST_CASE("Untimed Temporal Operations") {
  auto manager = std::make_shared<reelay::binding_manager>();
  auto opts = reelay::basic_options().with_data_manager(manager);
  SECTION("Once") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 4}, {"state", "open"}, {"arg1", "c"}});
    sequence.push_back(input_t{{"time", 6}, {"state", "close"}, {"arg1", "c"}});
    sequence.push_back(input_t{{"time", 7}, {"state", "close"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 9}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "once{state: open, arg1: c}", opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 4), f));
    expected.add(std::make_pair(interval::left_open(4, 9), t));

    CHECK(result == expected);
  }

  SECTION("Once") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 4}, {"state", "open"}, {"arg1", "c"}});
    sequence.push_back(input_t{{"time", 6}, {"state", "close"}, {"arg1", "c"}});
    sequence.push_back(input_t{{"time", 7}, {"state", "close"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 9}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "once{state: open, arg1: *val}", opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto val_a = manager->assign("val", "a");
    auto val_c = manager->assign("val", "c");

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 4), val_a));
    expected.add(std::make_pair(interval::left_open(4, 9), val_a + val_c));

    CHECK(result == expected);
  }

  SECTION("Historically") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 4}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 6}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 7}, {"state", "close"}, {"arg1", "c"}});
    sequence.push_back(input_t{{"time", 9}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "historically{state: open, arg1: a}", opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 7), t));
    expected.add(std::make_pair(interval::left_open(7, 9), f));

    CHECK(result == expected);
  }

  SECTION("Historically") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 4}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 6}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 7}, {"state", "close"}, {"arg1", "c"}});
    sequence.push_back(input_t{{"time", 9}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "historically{state: open, arg1: *val}", opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto val_a = manager->assign("val", "a");

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 7), val_a));
    expected.add(std::make_pair(interval::left_open(7, 9), f));

    CHECK(result == expected);
  }

  SECTION("Since") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"state", "arrived"}});
    sequence.push_back(input_t{{"time", 1}, {"state", "open"}, {"arg1", "b"}});
    sequence.push_back(input_t{{"time", 4}, {"state", "open"}, {"arg1", "c"}});
    sequence.push_back(input_t{{"time", 6}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 7}, {"state", "close"}});
    sequence.push_back(input_t{{"time", 9}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "{state: open, arg1: *} since {state: arrived}", opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 7), t));
    expected.add(std::make_pair(interval::left_open(7, 9), f));

    CHECK(result == expected);
  }

  SECTION("Since") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(
        input_t{{"time", 0}, {"state", "arrived"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 1}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 4}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 6}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 7}, {"state", "close"}});
    sequence.push_back(input_t{{"time", 9}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "{state: open, arg1: *arg} since {state: arrived, arg1: *arg}", opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto arg_a = manager->assign("arg", "a");

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 7), arg_a));
    expected.add(std::make_pair(interval::left_open(7, 9), f));

    CHECK(result == expected);
  }

  SECTION("Since") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}});
    sequence.push_back(input_t{
        {"time", 3},
        {"visitor1", "alice"},
        {"visitor2", "charlotte"},
        {"visiting1", "alice"},
        {"where", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 9},
        {"visiting1", "alice"},
        {"visiting2", "charlotte"},
        {"where", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 13},
        {"visiting1", "alice"},
        {"visiting2", "charlotte"},
        {"where", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 21},
        {"visiting1", "alice"},
        {"visiting2", "charlotte"},
        {"where", "wonderland"}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "({visiting1: *name} or {visiting2: *name}) "
        "since ({visitor1: *name} or {visitor2: *name})",
        opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto alice = manager->assign("name", "alice");
    auto charlotte = manager->assign("name", "charlotte");

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 3), f));
    expected.add(std::make_pair(interval::left_open(3, 21), alice + charlotte));

    CHECK(result == expected);
  }
}

TEST_CASE("Timed Temporal Operations") {
  auto manager = std::make_shared<reelay::binding_manager>();
  auto opts = reelay::basic_options().with_data_manager(manager);
  SECTION("Timed Once") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 1}, {"state", "open"}, {"arg1", "c"}});
    sequence.push_back(input_t{{"time", 3}, {"state", "close"}, {"arg1", "c"}});
    sequence.push_back(input_t{{"time", 6}, {"state", "close"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 7}, {"state", "close"}, {"arg1", "c"}});
    sequence.push_back(input_t{{"time", 9}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(
        input_t{{"time", 12}, {"state", "close"}, {"arg1", "c"}});
    sequence.push_back(input_t{{"time", 17}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "once[2:8]{state: open, arg1: c}", opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 3), f));
    expected.add(std::make_pair(interval::left_open(3, 11), t));
    expected.add(std::make_pair(interval::left_open(11, 17), f));

    CHECK(result == expected);
  }

  SECTION("Timed Once Zero") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 1}, {"state", "open"}, {"arg1", "c"}});
    sequence.push_back(input_t{{"time", 3}, {"state", "close"}, {"arg1", "c"}});
    sequence.push_back(input_t{{"time", 6}, {"state", "close"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 7}, {"state", "close"}, {"arg1", "c"}});
    sequence.push_back(input_t{{"time", 9}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(
        input_t{{"time", 12}, {"state", "close"}, {"arg1", "c"}});
    sequence.push_back(input_t{{"time", 17}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "once[:8]{state: open, arg1: c}", opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 1), f));
    expected.add(std::make_pair(interval::left_open(1, 11), t));
    expected.add(std::make_pair(interval::left_open(11, 17), f));

    CHECK(result == expected);
  }

  SECTION("Timed Once Inf") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 1}, {"state", "open"}, {"arg1", "c"}});
    sequence.push_back(input_t{{"time", 3}, {"state", "close"}, {"arg1", "c"}});
    sequence.push_back(input_t{{"time", 6}, {"state", "close"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 7}, {"state", "close"}, {"arg1", "c"}});
    sequence.push_back(input_t{{"time", 9}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(
        input_t{{"time", 12}, {"state", "close"}, {"arg1", "c"}});
    sequence.push_back(input_t{{"time", 17}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "once[2:]{state: open, arg1: c}", opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 3), f));
    expected.add(std::make_pair(interval::left_open(3, 17), t));

    CHECK(result == expected);
  }

  SECTION("Timed Historically") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"state", "open"}, {"arg1", "b"}});
    sequence.push_back(input_t{{"time", 1}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 3}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 4}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 7}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 11}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(
        input_t{{"time", 12}, {"state", "close"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 14}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "historically[:8]{state: open, arg1: a}", opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 9), f));
    expected.add(std::make_pair(interval::left_open(9, 12), t));
    expected.add(std::make_pair(interval::left_open(12, 14), f));

    CHECK(result == expected);
  }

  SECTION("Timed Historically") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 3}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 4}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 7}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 11}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 12}, {"state", "close"}});
    sequence.push_back(input_t{{"time", 19}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "historically[4:]{state: open, arg1: a}", opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 16), t));
    expected.add(std::make_pair(interval::left_open(16, 19), f));

    CHECK(result == expected);
  }

  SECTION("Timed Historically") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"state", "open"}, {"arg1", "b"}});
    sequence.push_back(input_t{{"time", 3}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 4}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 7}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 11}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 12}, {"state", "close"}});
    sequence.push_back(input_t{{"time", 19}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "historically[2:4]{state: open, arg1: a}", opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 2), t));
    expected.add(std::make_pair(interval::left_open(2, 7), f));
    expected.add(std::make_pair(interval::left_open(7, 14), t));
    expected.add(std::make_pair(interval::left_open(14, 19), f));

    CHECK(result == expected);
  }

  SECTION("Timed Since") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}});
    sequence.push_back(input_t{
        {"time", 1},
        {"visitor1", "alice"},
        {"visitor2", "charlotte"},
        {"visiting1", "alice"},
        {"where", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 3},
        {"visitor1", ""},
        {"visitor2", ""},
        {"visiting1", "alice"},
        {"visiting2", "charlotte"},
        {"where", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 9},
        {"visiting1", "alice"},
        {"visiting2", "charlotte"},
        {"where", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 13},
        {"visiting1", "alice"},
        {"visiting2", "charlotte"},
        {"where", "wonderland"}});
    sequence.push_back(input_t{{"time", 21}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "({visiting1: *name} or {visiting2: *name}) "
        "since[2:12] ({visitor1: *name} or {visitor2: *name})",
        opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto alice = manager->assign("name", "alice");
    auto charlotte = manager->assign("name", "charlotte");

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 3), f));
    expected.add(std::make_pair(interval::left_open(3, 5), alice));
    expected.add(std::make_pair(interval::left_open(5, 15), alice + charlotte));
    expected.add(std::make_pair(interval::left_open(15, 21), f));

    CHECK(result == expected);
  }

  SECTION("Timed Since Inf") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}});
    sequence.push_back(input_t{
        {"time", 1},
        {"visitor1", "alice"},
        {"visitor2", "charlotte"},
        {"visiting1", "alice"},
        {"where", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 3},
        {"visitor1", "alice"},
        {"visitor2", "charlotte"},
        {"visiting1", "alice"},
        {"visiting2", "charlotte"},
        {"where", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 9},
        {"visiting1", "alice"},
        {"visiting2", "charlotte"},
        {"where", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 13},
        {"visiting1", "alice"},
        {"visiting2", "charlotte"},
        {"where", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 21}, {"visiting1", "charlotte"}, {"where", "wonderland"}});
    sequence.push_back(input_t{{"time", 23}});

    auto net1 = reelay::dense_timed_data_network<time_type, input_t>::make(
        "({visiting1: *name} or {visiting2: *name}) "
        "since[2:] ({visitor1: *name} or {visitor2: *name})",
        opts);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1.update(row);
      for (const auto &intv : net1.output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto alice = manager->assign("name", "alice");
    auto charlotte = manager->assign("name", "charlotte");

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 3), f));
    expected.add(std::make_pair(interval::left_open(3, 5), alice));
    expected.add(std::make_pair(interval::left_open(5, 21), alice + charlotte));
    expected.add(std::make_pair(interval::left_open(21, 23), charlotte));

    CHECK(result == expected);
  }
}
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
using time_t = int64_t;

using interval = reelay::interval<time_t>;
using interval_map = reelay::data_interval_map<time_t>;

TEST_CASE("Atoms") {
  SECTION("Proposition_ZeroHoldForward") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"x1", true}});
    sequence.push_back(input_t{{"time", 31}});
    sequence.push_back(input_t{{"time", 33}, {"x1", false}});
    sequence.push_back(input_t{{"time", 35}, {"x1", false}});
    sequence.push_back(input_t{{"time", 40}});
    sequence.push_back(input_t{{"time", 52}, {"x1", true}});
    sequence.push_back(input_t{{"time", 55}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("{x1}", extra_args);

    auto result1 = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("{x1: *}", extra_args);

    auto result1 = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("{x1: true}", extra_args);

    auto result1 = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("{x1: false}", extra_args);

    auto result1 = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("{x1: veritas}", extra_args);

    auto result1 = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("{x1: 12.5}", extra_args);

    auto result1 = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("{x1 < 4}", extra_args);

    auto result1 = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("{x1 <= 4}", extra_args);

    auto result1 = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("{x1 > 4}", extra_args);

    auto result1 = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("{x1 >= 4}", extra_args);

    auto result1 = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

  // SECTION("List Proposition 1") {

  //   std::vector<input_t> sequence = std::vector<input_t>();

  //   sequence.push_back(input_t{{"time", 0}, {"event", {"open"}}});
  //   sequence.push_back(input_t{{"time", 3}, {"event", {"open"}}});
  //   sequence.push_back(input_t{{"time", 6}, {"event", {"open"}}});
  //   sequence.push_back(input_t{{"time", 10}, {"event", {"closed"}}});
  //   sequence.push_back(input_t{{"time", 14}, {"event", {"closed"}}});

  //   auto manager = std::make_shared<reelay::binding_manager>();
  //   reelay::kwargs extra_args = {{"manager", manager}};

  //   auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
  //       input_t>::from_temporal_logic("event::{$0: open}", extra_args);

  //   auto result = interval_map();

  //   for (const auto &row : sequence) {
  //     net1->update(row);
  //     for (const auto &intv : net1->output()) {
  //       result.add(intv);
  //     }
  //   }

  //   auto t = manager->one();
  //   auto f = manager->zero();

  //   auto expected = interval_map();
  //   expected.add(std::make_pair(interval::left_open(0, 10), t));
  //   expected.add(std::make_pair(interval::left_open(10, 14), f));

  //   CHECK(result == expected);
  // }

  // SECTION("List Proposition 2") {

  //   std::vector<input_t> sequence = std::vector<input_t>();

  //   sequence.push_back(input_t{{"time", 0}, {"event", {"open", "abc"}}});
  //   sequence.push_back(input_t{{"time", 3}, {"event", {"open", "cdf"}}});
  //   sequence.push_back(input_t{{"time", 6}, {"event", {"open", "efg"}}});
  //   sequence.push_back(input_t{{"time", 10}, {"event", {"closed"}}});
  //   sequence.push_back(input_t{{"time", 14}, {"event", {"closed"}}});

  //   auto manager = std::make_shared<reelay::binding_manager>();
  //   reelay::kwargs extra_args = {{"manager", manager}};

  //   auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
  //       input_t>::from_temporal_logic("event::{$0: open, $1: abc}",
  //       extra_args);

  //   auto result = interval_map();

  //   for (const auto &row : sequence) {
  //     net1->update(row);
  //     for (const auto &intv : net1->output()) {
  //       result.add(intv);
  //     }
  //   }

  //   auto t = manager->one();
  //   auto f = manager->zero();

  //   auto expected = interval_map();
  //   expected.add(std::make_pair(interval::left_open(0, 3), t));
  //   expected.add(std::make_pair(interval::left_open(3, 14), f));

  //   CHECK(result == expected);
  // }

  // SECTION("List Proposition 3") {

  //
  //   std::vector<input_t> sequence = std::vector<input_t>();

  //   sequence.push_back(input_t{1, "open", "abc"});
  //   sequence.push_back(input_t{3, "open", "abc"});
  //   sequence.push_back(input_t{6, "open", "cdf"});
  //   sequence.push_back(input_t{10, "closed"});
  //   sequence.push_back(input_t{14, "closed"});

  //   auto manager = std::make_shared<reelay::binding_manager>();
  //   reelay::kwargs extra_args = {{"manager", manager}};

  //   auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
  //       input_t>::from_temporal_logic("[open, *filename]", extra_args);

  //   auto result = interval_map();

  //   for (const auto &row : sequence) {
  //     net1->update(row);
  //     for (const auto &intv : net1->output()) {
  //       result.add(intv);
  //     }
  //   }

  //   auto t = manager->one();
  //   auto f = manager->zero();

  //   auto datum1 = manager->assign("filename", "abc");
  //   auto datum2 = manager->assign("filename", "cdf");

  //   auto expected = interval_map();
  //   expected.add(std::make_pair(interval::left_open(0, 3), datum1));
  //   expected.add(std::make_pair(interval::left_open(3, 6), datum2));
  //   expected.add(std::make_pair(interval::left_open(6, 14), f));

  //   CHECK(result == expected);
  // }

  // SECTION("List Proposition Event") {

  //
  //   std::vector<input_t> sequence = std::vector<input_t>();

  //   sequence.push_back(input_t{1, "open"});
  //   sequence.push_back(input_t{3, "open"});
  //   sequence.push_back(input_t{6, "open"});
  //   sequence.push_back(input_t{10, "closed"});
  //   sequence.push_back(input_t{14, "closed"});

  //   auto manager = std::make_shared<reelay::binding_manager>();
  //   reelay::kwargs extra_args = {{"manager", manager}};

  //   auto net1 = reelay::detail::dense_timed<time_t,
  //   1>::unordered_data::network<
  //       input_t>::from_temporal_logic("[open]", extra_args);

  //   auto result = interval_map();

  //   for (const auto &row : sequence) {
  //     net1->update(row);
  //     for (const auto &intv : net1->output()) {
  //       result.add(intv);
  //     }
  //   }

  //   auto t = manager->one();
  //   auto f = manager->zero();

  //   auto expected = interval_map();
  //   expected.add(std::make_pair(interval::left_open(0, 1), f));
  //   expected.add(std::make_pair(interval::closed(1, 1), t));
  //   expected.add(std::make_pair(interval::open(1, 3), f));
  //   expected.add(std::make_pair(interval::closed(3, 3), t));
  //   expected.add(std::make_pair(interval::open(3, 6), f));
  //   expected.add(std::make_pair(interval::closed(6, 6), t));
  //   expected.add(std::make_pair(interval::left_open(6, 14), f));

  //   CHECK(result == expected);
  // }

  SECTION("Record Proposition 0") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0},
                               {"event", "access"},
                               {"user", "alice"},
                               {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", 3},
                               {"event", "access"},
                               {"user", "alice"},
                               {"file", "feed_your_head"}});
    sequence.push_back(input_t{{"time", 6},
                               {"event", "access"},
                               {"user", "alice"},
                               {"file", "wonderland"},
                               {"dummy_field", "v2"}});
    sequence.push_back(input_t{{"time", 9}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::
        network<input_t>::from_temporal_logic(
            "{event: access, user: alice, file: wonderland}", extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    sequence.push_back(input_t{{"time", 0},
                               {"event", "access"},
                               {"user", "alice"},
                               {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", 3},
                               {"event", "access"},
                               {"user", "bob"},
                               {"file", "feed_your_head"}});
    sequence.push_back(input_t{{"time", 6},
                               {"event", "access"},
                               {"user", "charlotte"},
                               {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", 9}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::
        network<input_t>::from_temporal_logic(
            "{event: access, user: *name, file: wonderland}", extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    sequence.push_back(input_t{{"time", 0},
                               {"event", "access"},
                               {"user", "alice"},
                               {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", 3},
                               {"event", "access"},
                               {"user", "bob"},
                               {"file", "feed_your_head"}});
    sequence.push_back(input_t{{"time", 6},
                               {"event", "access"},
                               {"user", "charlotte"},
                               {"file", "wonderland"}});
    sequence.push_back(
        input_t{{"time", 9}, {"event", "access"}, {"user", "charlotte"}});
    sequence.push_back(input_t{{"time", 11}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("{event: access, user: *name, file:*}",
                                      extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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
    sequence.push_back(input_t{{"time", 6},
                               {"event", "access"},
                               {"user", "charlotte"},
                               {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", 9}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("{file: wonderland}", extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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
    sequence.push_back(input_t{{"time", 6},
                               {"event", "access"},
                               {"user", "charlotte"},
                               {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", 9}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("{file: wonderland, user:*}", extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

TEST_CASE("Nested Inputs") {
  SECTION("Deep Object") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"obj", {{"x", 2}}}});
    sequence.push_back(input_t{{"time", 10}, {"obj", {{"x", 4}}}});
    sequence.push_back(input_t{{"time", 20}});
    sequence.push_back(input_t{{"time", 30}, {"obj", {{"x", 6}}}});
    sequence.push_back(input_t{{"time", 40}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("obj::{x > 4}", extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 30), f));
    expected.add(std::make_pair(interval::left_open(30, 40), t));

    CHECK(result == expected);
  }

  SECTION("Deep Object 2") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{
        {"time", 0}, {"obj1", {{"obj2", {{"flag1", false}, {"flag2", 2}}}}}});
    sequence.push_back(input_t{
        {"time", 10}, {"obj1", {{"obj2", {{"flag1", false}, {"flag2", 4}}}}}});
    sequence.push_back(input_t{{"time", 20}});
    sequence.push_back(
        input_t{{"time", 30}, {"obj1", {{"obj2", {{"flag2", 6}}}}}});
    sequence.push_back(input_t{{"time", 40}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("obj1::obj2::{flag2 > 4}", extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 30), f));
    expected.add(std::make_pair(interval::left_open(30, 40), t));

    CHECK(result == expected);
  }

  // SECTION("Deep List Any") {
  //   std::vector<input_t> sequence = std::vector<input_t>();

  //   sequence.push_back(input_t{
  //       {"time", 0},
  //       {"obj1", {{"obj2", {{{"a", 7}, {"b", 3}}, {{"a", 9}, {"b", 2}}}}}}});
  //   sequence.push_back(input_t{
  //       {"time", 10},
  //       {"obj1", {{"obj2", {{{"a", 7}, {"b", 3}}, {{"a", 5}, {"b", 2}}}}}}});
  //   sequence.push_back(input_t{
  //       {"time", 20},
  //       {"obj1", {{"obj2", {{{"a", 7}, {"b", 3}}, {{"a", 5}, {"b", 2}}}}}}});
  //   sequence.push_back(input_t{{"time", 40}});

  //   auto net1 =
  //   reelay::detail::dense_timed<time_t>::unordered_data::network<
  //       input_t>::from_temporal_logic("obj1::obj2::any{a < 6}");

  //   auto result = interval_map();

  //   for (const auto &s : sequence) {
  //     net1->update(s);
  //     result = result | net1->output();
  //   }

  //   auto expected = interval_map();
  //   expected.add(std::make_pair(interval::left_open(0, 10), -1));
  //   expected.add(std::make_pair(interval::left_open(10, 40), 1));

  //   CHECK(result == expected);
  // }

  // SECTION("Deep List All") {
  //   std::vector<input_t> sequence = std::vector<input_t>();

  //   sequence.push_back(input_t{
  //       {"time", 0},
  //       {"obj1", {{"obj2", {{{"a", 7}, {"b", 3}}, {{"a", 9}, {"b", 2}}}}}}});
  //   sequence.push_back(input_t{
  //       {"time", 10},
  //       {"obj1", {{"obj2", {{{"a", 7}, {"b", 3}}, {{"a", 5}, {"b", 2}}}}}}});
  //   sequence.push_back(input_t{
  //       {"time", 20},
  //       {"obj1", {{"obj2", {{{"a", 7}, {"b", 3}}, {{"a", 5}, {"b", 0}}}}}}});
  //   sequence.push_back(input_t{{"time", 40}});

  //   auto net1 =
  //   reelay::detail::dense_timed<time_t>::unordered_data::network<
  //       input_t>::from_temporal_logic("obj1::obj2::all{b > 1}");

  //   auto result = interval_map();

  //   for (const auto &s : sequence) {
  //     net1->update(s);
  //     result = result | net1->output();
  //   }

  //   auto expected = interval_map();
  //   expected.add(std::make_pair(interval::left_open(0, 10), 1));

  //   CHECK(result == expected);
  // }
}

TEST_CASE("Boolean Operations") {
  SECTION("Negation") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0},
                               {"event", "access"},
                               {"user", "alice"},
                               {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", 1},
                               {"event", "access"},
                               {"user", "alice"},
                               {"file", "feed_your_head"}});
    sequence.push_back(input_t{{"time", 3},
                               {"event", "access"},
                               {"user", "charlotte"},
                               {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", 6},
                               {"event", "logout"},
                               {"user", "alice"},
                               {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", 10}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("!{event: access, user: alice}",
                                      extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    sequence.push_back(input_t{{"time", 0},
                               {"event", "access"},
                               {"user", "alice"},
                               {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", 1},
                               {"event", "access"},
                               {"user", "alice"},
                               {"file", "feed_your_head"}});
    sequence.push_back(input_t{{"time", 3},
                               {"event", "access"},
                               {"user", "charlotte"},
                               {"file", "wonderland"}});
    sequence.push_back(
        input_t{{"time", 6}, {"event", "logout"}, {"user", "alice"}});
    sequence.push_back(input_t{{"time", 10}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::
        network<input_t>::from_temporal_logic(
            "{event: *e,     user: alice, file: *f} and"
            "{event: access, user: *u,    file: wonderland}",
            extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto alice_in_wland = manager->assign("e", "access") *
                          manager->assign("u", "alice") *
                          manager->assign("f", "wonderland");

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 1), alice_in_wland));
    expected.add(std::make_pair(interval::left_open(1, 10), f));

    CHECK(result == expected);
  }

  SECTION("Disjunction") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0},
                               {"event", "access"},
                               {"user", "alice"},
                               {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", 1},
                               {"event", "access"},
                               {"user", "alice"},
                               {"file", "feed_your_head"}});
    sequence.push_back(input_t{{"time", 3},
                               {"event", "access"},
                               {"user", "charlotte"},
                               {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", 6},
                               {"event", "access"},
                               {"user", "square"},
                               {"file", "flatland"}});
    sequence.push_back(input_t{{"time", 10}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::
        network<input_t>::from_temporal_logic(
            "{event: *e,     user: alice, file: *f} or"
            "{event: access, user: *u,    file: wonderland}",
            extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto datum1 =
        (manager->assign("e", "access") * manager->assign("f", "wonderland")) +
        manager->assign("u", "alice");
    auto datum2 =
        manager->assign("e", "access") * manager->assign("f", "feed_your_head");
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

    sequence.push_back(input_t{{"time", 0},
                               {"event", "access"},
                               {"user", "alice"},
                               {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", 1},
                               {"event", "access"},
                               {"user", "alice"},
                               {"file", "feed_your_head"}});
    sequence.push_back(input_t{{"time", 3},
                               {"event", "access"},
                               {"user", "charlotte"},
                               {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", 6},
                               {"event", "access"},
                               {"user", "square"},
                               {"file", "flatland"}});
    sequence.push_back(input_t{{"time", 10}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::
        network<input_t>::from_temporal_logic(
            "not{event: *e, user: alice, file: *f} implies"
            "{event: access, user: *u, file: wonderland}",
            extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto datum1 =
        (manager->assign("e", "access") * manager->assign("f", "wonderland")) +
        manager->assign("u", "alice");
    auto datum2 =
        manager->assign("e", "access") * manager->assign("f", "feed_your_head");
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

    sequence.push_back(input_t{{"time", 0},
                               {"event", "access"},
                               {"user", "alice"},
                               {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", 1},
                               {"event", "access"},
                               {"user", "alice"},
                               {"file", "feed_your_head"}});
    sequence.push_back(input_t{{"time", 3},
                               {"event", "access"},
                               {"user", "charlotte"},
                               {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", 6},
                               {"event", "access"},
                               {"user", "square"},
                               {"file", "flatland"}});
    sequence.push_back(input_t{{"time", 10}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::
        network<input_t>::from_temporal_logic(
            "exists[e]. {event: *e, user: alice, file: *f}", extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    sequence.push_back(input_t{{"time", 0},
                               {"event", "access"},
                               {"user", "alice"},
                               {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", 1},
                               {"event", "access"},
                               {"user", "alice"},
                               {"file", "feed_your_head"}});
    sequence.push_back(input_t{{"time", 3},
                               {"event", "access"},
                               {"user", "charlotte"},
                               {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", 6},
                               {"event", "access"},
                               {"user", "square"},
                               {"file", "flatland"}});
    sequence.push_back(input_t{{"time", 10}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::
        network<input_t>::from_temporal_logic(
            "not(forall[e].(not{event: *e, user: alice, file: *f}))",
            extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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
  SECTION("Once") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", 0}, {"state", "open"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 4}, {"state", "open"}, {"arg1", "c"}});
    sequence.push_back(input_t{{"time", 6}, {"state", "close"}, {"arg1", "c"}});
    sequence.push_back(input_t{{"time", 7}, {"state", "close"}, {"arg1", "a"}});
    sequence.push_back(input_t{{"time", 9}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("once{state: open, arg1: c}", extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("once{state: open, arg1: *val}",
                                      extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("historically{state: open, arg1: a}",
                                      extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("historically{state: open, arg1: *val}",
                                      extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::
        network<input_t>::from_temporal_logic(
            "{state: open, arg1: *} since {state: arrived}", extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::
        network<input_t>::from_temporal_logic(
            "{state: open, arg1: *arg} since {state: arrived, arg1: *arg}",
            extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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
    sequence.push_back(input_t{{"time", 3},
                               {"visitor1", "alice"},
                               {"visitor2", "charlotte"},
                               {"visiting1", "alice"},
                               {"where", "wonderland"}});
    sequence.push_back(input_t{{"time", 9},
                               {"visiting1", "alice"},
                               {"visiting2", "charlotte"},
                               {"where", "wonderland"}});
    sequence.push_back(input_t{{"time", 13},
                               {"visiting1", "alice"},
                               {"visiting2", "charlotte"},
                               {"where", "wonderland"}});
    sequence.push_back(input_t{{"time", 21},
                               {"visiting1", "alice"},
                               {"visiting2", "charlotte"},
                               {"where", "wonderland"}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::
        network<input_t>::from_temporal_logic(
            "({visiting1: *name} or {visiting2: *name}) "
            "since ({visitor1: *name} or {visitor2: *name})",
            extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("once[2:8]{state: open, arg1: c}",
                                      extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("once[:8]{state: open, arg1: c}",
                                      extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("once[2:]{state: open, arg1: c}",
                                      extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("historically[:8]{state: open, arg1: a}",
                                      extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("historically[4:]{state: open, arg1: a}",
                                      extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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
    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::network<
        input_t>::from_temporal_logic("historically[2:4]{state: open, arg1: a}",
                                      extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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
    sequence.push_back(input_t{{"time", 1},
                               {"visitor1", "alice"},
                               {"visitor2", "charlotte"},
                               {"visiting1", "alice"},
                               {"where", "wonderland"}});
    sequence.push_back(input_t{{"time", 3},
                               {"visitor1", ""},
                               {"visitor2", ""},
                               {"visiting1", "alice"},
                               {"visiting2", "charlotte"},
                               {"where", "wonderland"}});
    sequence.push_back(input_t{{"time", 9},
                               {"visiting1", "alice"},
                               {"visiting2", "charlotte"},
                               {"where", "wonderland"}});
    sequence.push_back(input_t{{"time", 13},
                               {"visiting1", "alice"},
                               {"visiting2", "charlotte"},
                               {"where", "wonderland"}});
    sequence.push_back(input_t{{"time", 21}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::
        network<input_t>::from_temporal_logic(
            "({visiting1: *name} or {visiting2: *name}) "
            "since[2:12] ({visitor1: *name} or {visitor2: *name})",
            extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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
    sequence.push_back(input_t{{"time", 1},
                               {"visitor1", "alice"},
                               {"visitor2", "charlotte"},
                               {"visiting1", "alice"},
                               {"where", "wonderland"}});
    sequence.push_back(input_t{{"time", 3},
                               {"visitor1", "alice"},
                               {"visitor2", "charlotte"},
                               {"visiting1", "alice"},
                               {"visiting2", "charlotte"},
                               {"where", "wonderland"}});
    sequence.push_back(input_t{{"time", 9},
                               {"visiting1", "alice"},
                               {"visiting2", "charlotte"},
                               {"where", "wonderland"}});
    sequence.push_back(input_t{{"time", 13},
                               {"visiting1", "alice"},
                               {"visiting2", "charlotte"},
                               {"where", "wonderland"}});
    sequence.push_back(input_t{
        {"time", 21}, {"visiting1", "charlotte"}, {"where", "wonderland"}});
    sequence.push_back(input_t{{"time", 23}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::detail::dense_timed<time_t>::unordered_data::
        network<input_t>::from_temporal_logic(
            "({visiting1: *name} or {visiting2: *name}) "
            "since[2:] ({visitor1: *name} or {visitor2: *name})",
            extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
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
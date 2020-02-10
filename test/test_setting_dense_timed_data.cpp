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

using time_t = int64_t;
using value_t = BDD;
using interval = reelay::interval<time_t>;
using interval_map = reelay::data_interval_map<time_t>;

TEST_CASE("Atoms") {

  SECTION("List Proposition 1") {

    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"1", "open"});
    sequence.push_back(input_t{"3", "open"});
    sequence.push_back(input_t{"6", "open"});
    sequence.push_back(input_t{"10", "closed"});
    sequence.push_back(input_t{"14", "closed"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("[open]", extra_args);

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
    expected.add(std::make_pair(interval::left_open(0, 6), t));
    expected.add(std::make_pair(interval::left_open(6, 14), f));

    CHECK(result == expected);
  }

  SECTION("List Proposition 2") {

    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"1", "open", "abc"});
    sequence.push_back(input_t{"3", "open", "cdf"});
    sequence.push_back(input_t{"6", "open", "efg"});
    sequence.push_back(input_t{"10", "closed"});
    sequence.push_back(input_t{"14", "closed"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("[open, *]", extra_args);

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
    expected.add(std::make_pair(interval::left_open(0, 6), t));
    expected.add(std::make_pair(interval::left_open(6, 14), f));

    CHECK(result == expected);
  }

  SECTION("List Proposition 3") {

    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"1", "open", "abc"});
    sequence.push_back(input_t{"3", "open", "abc"});
    sequence.push_back(input_t{"6", "open", "cdf"});
    sequence.push_back(input_t{"10", "closed"});
    sequence.push_back(input_t{"14", "closed"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("[open, *filename]", extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto datum1 = manager->assign("filename", "abc");
    auto datum2 = manager->assign("filename", "cdf");

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 3), datum1));
    expected.add(std::make_pair(interval::left_open(3, 6), datum2));
    expected.add(std::make_pair(interval::left_open(6, 14), f));

    CHECK(result == expected);
  }

  SECTION("Record Proposition 0") {

    using input_t = std::unordered_map<std::string, std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "3"},
                               {"event", "access"},
                               {"user", "alice"},
                               {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", "6"},
                               {"event", "access"},
                               {"user", "alice"},
                               {"file", "feed_your_head"}});
    sequence.push_back(input_t{{"time", "9"},
                               {"event", "access"},
                               {"user", "alice"},
                               {"file", "wonderland"},
                               {"dummy_field", "v2"}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<input_t>::
        from_temporal_logic("{event: access, user: alice, file: wonderland}",
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
    expected.add(std::make_pair(interval::left_open(0, 3), t));
    expected.add(std::make_pair(interval::left_open(3, 6), f));
    expected.add(std::make_pair(interval::left_open(6, 9), t));

    CHECK(result == expected);
  }

  SECTION("Record Proposition 1") {

    using input_t = std::unordered_map<std::string, std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "3"},
                               {"event", "access"},
                               {"user", "alice"},
                               {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", "6"},
                               {"event", "access"},
                               {"user", "bob"},
                               {"file", "feed_your_head"}});
    sequence.push_back(input_t{{"time", "9"},
                               {"event", "access"},
                               {"user", "charlotte"},
                               {"file", "wonderland"}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<input_t>::
        from_temporal_logic("{event: access, user: *name, file: wonderland}",
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
    auto d3 = manager->assign("name", "charlotte");

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 3), d1));
    expected.add(std::make_pair(interval::left_open(3, 6), f));
    expected.add(std::make_pair(interval::left_open(6, 9), d3));

    CHECK(result == expected);
  }

  SECTION("Record Proposition 2") {

    using input_t = std::unordered_map<std::string, std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "3"},
                               {"event", "access"},
                               {"user", "alice"},
                               {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", "6"},
                               {"event", "access"},
                               {"user", "bob"},
                               {"file", "feed_your_head"}});
    sequence.push_back(input_t{{"time", "9"},
                               {"event", "access"},
                               {"user", "charlotte"},
                               {"file", "wonderland"}});
    sequence.push_back(
        input_t{{"time", "11"}, {"event", "access"}, {"user", "charlotte"}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<
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

    using input_t = std::unordered_map<std::string, std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(
        input_t{{"time", "3"}, {"event", "access"}, {"user", "alice"}});
    sequence.push_back(
        input_t{{"time", "6"}, {"event", "access"}, {"user", "bob"}});
    sequence.push_back(input_t{{"time", "9"},
                               {"event", "access"},
                               {"user", "charlotte"},
                               {"file", "wonderland"}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<
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

    using input_t = std::unordered_map<std::string, std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(
        input_t{{"time", "3"}, {"event", "access"}, {"user", "alice"}});
    sequence.push_back(
        input_t{{"time", "6"}, {"event", "access"}, {"file", "wonderland"}});
    sequence.push_back(input_t{{"time", "9"},
                               {"event", "access"},
                               {"user", "charlotte"},
                               {"file", "wonderland"}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<
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

TEST_CASE("Boolean Operations") {

  SECTION("Negation") {

    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"1", "access", "alice", "wonderland"});
    sequence.push_back(input_t{"3", "access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"6", "access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"10", "logout", "alice"});
    sequence.push_back(input_t{"14", "meet", "bob", "humpty", "dumpty"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("![access, alice, *]", extra_args);

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
    expected.add(std::make_pair(interval::left_open(1, 3), t));
    expected.add(std::make_pair(interval::left_open(3, 6), f));
    expected.add(std::make_pair(interval::left_open(6, 14), t));

    CHECK(result == expected);
  }

  SECTION("Conjunction") {

    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"1", "access", "alice", "wonderland"});
    sequence.push_back(input_t{"3", "access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"6", "access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"10", "logout", "alice"});
    sequence.push_back(input_t{"14", "meet", "bob", "humpty", "dumpty"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<input_t>::
        from_temporal_logic(
            "[*event, alice, *file] and [access, *user, wonderland]",
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

    auto alice_in_wland = manager->assign("event", "access") *
                          manager->assign("user", "alice") *
                          manager->assign("file", "wonderland");

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 1), alice_in_wland));
    expected.add(std::make_pair(interval::left_open(1, 14), f));

    CHECK(result == expected);
  }

  SECTION("Disjunction") {

    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"1", "access", "alice", "wonderland"});
    sequence.push_back(input_t{"3", "access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"6", "access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"10", "logout", "alice"});
    sequence.push_back(input_t{"14", "meet", "bob", "humpty", "dumpty"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<input_t>::
        from_temporal_logic(
            "[*event, alice, *file] or [access, *user, wonderland]",
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

    auto datum1 = (manager->assign("event", "access") *
                   manager->assign("file", "wonderland")) +
                  manager->assign("user", "alice");

    auto datum3 = manager->assign("event", "access") *
                  manager->assign("file", "feed_your_head");

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 1), datum1));
    expected.add(std::make_pair(interval::left_open(1, 3), f));
    expected.add(std::make_pair(interval::left_open(3, 6), datum3));
    expected.add(std::make_pair(interval::left_open(6, 14), f));

    CHECK(result == expected);
  }

  SECTION("Existential Quantification") {

    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"1", "access", "alice", "wonderland"});
    sequence.push_back(input_t{"3", "access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"6", "access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"10", "logout", "alice"});
    sequence.push_back(input_t{"14", "meet", "bob", "humpty", "dumpty"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("exists[event]. [*event, alice, *file]",
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

    auto datum1 = manager->assign("file", "wonderland");
    auto datum3 = manager->assign("file", "feed_your_head");

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 1), datum1));
    expected.add(std::make_pair(interval::left_open(1, 3), f));
    expected.add(std::make_pair(interval::left_open(3, 6), datum3));
    expected.add(std::make_pair(interval::left_open(6, 14), f));

    CHECK(result == expected);
  }
}

TEST_CASE("Untimed Temporal Operations") {

  SECTION("Once") {
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"4", "open", "a"});
    sequence.push_back(input_t{"6", "open", "c"});
    sequence.push_back(input_t{"7", "close", "c"});
    sequence.push_back(input_t{"9", "close", "a"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("once([open, c])", extra_args);

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
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"4", "open", "a"});
    sequence.push_back(input_t{"6", "open", "c"});
    sequence.push_back(input_t{"7", "close", "c"});
    sequence.push_back(input_t{"9", "close", "a"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("once([open, *file])", extra_args);

    auto result = interval_map();

    for (const auto &row : sequence) {
      net1->update(row);
      for (const auto &intv : net1->output()) {
        result.add(intv);
      }
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto file_a = manager->assign("file", "a");
    auto file_c = manager->assign("file", "c");

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 4), file_a));
    expected.add(std::make_pair(interval::left_open(4, 9), file_a + file_c));

    CHECK(result == expected);
  }

  SECTION("Historically") {
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"4", "is_open", "a"});
    sequence.push_back(input_t{"6", "is_open", "a"});
    sequence.push_back(input_t{"7", "is_open", "a"});
    sequence.push_back(input_t{"9", "close", "a"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("historically([is_open, a])", extra_args);

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
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"4", "is_open", "a"});
    sequence.push_back(input_t{"6", "is_open", "a"});
    sequence.push_back(input_t{"7", "is_open", "a"});
    sequence.push_back(input_t{"9", "close", "a"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("historically([is_open, *file])",
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

    auto file_a = manager->assign("file", "a");

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 7), file_a));
    expected.add(std::make_pair(interval::left_open(7, 9), f));

    CHECK(result == expected);
  }

  SECTION("Since") {
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"1", "open", "a"});
    sequence.push_back(input_t{"4", "is_open", "a"});
    sequence.push_back(input_t{"6", "is_open", "a"});
    sequence.push_back(input_t{"7", "is_open", "a"});
    sequence.push_back(input_t{"9", "is_open", "b"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("[is_open, a] since [open, a]",
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

  SECTION("Since") {
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"1", "open", "a"});
    sequence.push_back(input_t{"4", "is_open", "a"});
    sequence.push_back(input_t{"6", "is_open", "a"});
    sequence.push_back(input_t{"7", "is_open", "a"});
    sequence.push_back(input_t{"9", "is_open", "b"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("[is_open, *file] since [open, *file]",
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

    auto file_a = manager->assign("file", "a");

    auto expected = interval_map();
    expected.add(std::make_pair(interval::left_open(0, 7), file_a));
    expected.add(std::make_pair(interval::left_open(7, 9), f));

    CHECK(result == expected);
  }

  SECTION("Since") {
    using input_t = std::unordered_map<std::string, std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "1"}});
    sequence.push_back(input_t{{"time", "3"},
                               {"visitor1", "alice"},
                               {"visitor2", "charlotte"},
                               {"visiting1", "alice"},
                               {"where", "wonderland"}});
    sequence.push_back(input_t{{"time", "9"},
                               {"visiting1", "alice"},
                               {"visiting2", "charlotte"},
                               {"where", "wonderland"}});
    sequence.push_back(input_t{{"time", "13"},
                               {"visiting1", "alice"},
                               {"visiting2", "charlotte"},
                               {"where", "wonderland"}});
    sequence.push_back(input_t{{"time", "21"},
                               {"visiting1", "alice"},
                               {"visiting2", "charlotte"},
                               {"where", "wonderland"}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<input_t>::
        from_temporal_logic(
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
    expected.add(std::make_pair(interval::left_open(0, 1), f));
    expected.add(std::make_pair(interval::left_open(1, 21), alice + charlotte));

    CHECK(result == expected);
  }
}

TEST_CASE("Timed Temporal Operations") {

  SECTION("Timed Once") {
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"1", "open", "a"});
    sequence.push_back(input_t{"3", "open", "c"});
    sequence.push_back(input_t{"6", "close", "c"});
    sequence.push_back(input_t{"7", "close", "a"});
    sequence.push_back(input_t{"9", "close", "c"});
    sequence.push_back(input_t{"11", "close", "a"});
    sequence.push_back(input_t{"17", "close", "c"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("once[2:8]([open, c])", extra_args);

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
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"1", "open", "a"});
    sequence.push_back(input_t{"3", "open", "c"});
    sequence.push_back(input_t{"6", "close", "c"});
    sequence.push_back(input_t{"7", "close", "a"});
    sequence.push_back(input_t{"9", "close", "c"});
    sequence.push_back(input_t{"11", "close", "a"});
    sequence.push_back(input_t{"17", "close", "c"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("once[:8]([open, c])", extra_args);

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
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"1", "open", "a"});
    sequence.push_back(input_t{"3", "open", "c"});
    sequence.push_back(input_t{"6", "close", "c"});
    sequence.push_back(input_t{"7", "close", "a"});
    sequence.push_back(input_t{"9", "close", "c"});
    sequence.push_back(input_t{"11", "close", "a"});
    sequence.push_back(input_t{"17", "close", "c"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("once[2:]([open, c])", extra_args);

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
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"1", "is_open", "b"});
    sequence.push_back(input_t{"3", "is_open", "a"});
    sequence.push_back(input_t{"4", "is_open", "a"});
    sequence.push_back(input_t{"7", "is_open", "a"});
    sequence.push_back(input_t{"11", "is_open", "a"});
    sequence.push_back(input_t{"12", "is_open", "a"});
    sequence.push_back(input_t{"14", "close", "a"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("historically[:8]([is_open, a])",
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
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"3", "is_open", "a"});
    sequence.push_back(input_t{"4", "is_open", "a"});
    sequence.push_back(input_t{"7", "is_open", "a"});
    sequence.push_back(input_t{"11", "is_open", "a"});
    sequence.push_back(input_t{"12", "is_open", "a"});
    sequence.push_back(input_t{"19", "close", "a"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("historically[4:]([is_open, a])",
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
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"3", "is_open", "b"});
    sequence.push_back(input_t{"5", "is_open", "a"});
    sequence.push_back(input_t{"7", "is_open", "a"});
    sequence.push_back(input_t{"9", "is_open", "a"});
    sequence.push_back(input_t{"13", "is_open", "a"});
    sequence.push_back(input_t{"21", "is_open", "a"});
    sequence.push_back(input_t{"23", "close", "a"});
    sequence.push_back(input_t{"24", "close", "a"});
    sequence.push_back(input_t{"34", "close", "a"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("historically[2:4]([is_open, a])",
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
    expected.add(std::make_pair(interval::left_open(7, 23), t));
    expected.add(std::make_pair(interval::left_open(23, 34), f));

    CHECK(result == expected);
  }

  SECTION("Timed Since") {
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"1", "open", "a"});
    sequence.push_back(input_t{"3", "is_open", "a"});
    sequence.push_back(input_t{"6", "is_open", "a"});
    sequence.push_back(input_t{"7", "is_open", "a"});
    sequence.push_back(input_t{"9", "is_open", "a"});
    sequence.push_back(input_t{"13", "is_open", "a"});
    sequence.push_back(input_t{"14", "is_open", "a"});
    sequence.push_back(input_t{"17", "is_open", "b"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("[is_open, a] since[2:10] [open, a]",
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

  SECTION("Timed Since") {
    using input_t = std::unordered_map<std::string, std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "1"}});
    sequence.push_back(input_t{{"time", "3"},
                               {"visitor1", "alice"},
                               {"visitor2", "charlotte"},
                               {"visiting1", "alice"},
                               {"where", "wonderland"}});
    sequence.push_back(input_t{{"time", "9"},
                               {"visiting1", "alice"},
                               {"visiting2", "charlotte"},
                               {"where", "wonderland"}});
    sequence.push_back(input_t{{"time", "13"},
                               {"visiting1", "alice"},
                               {"visiting2", "charlotte"},
                               {"where", "wonderland"}});
    sequence.push_back(input_t{{"time", "21"},
                               {"visiting1", "alice"},
                               {"visiting2", "charlotte"},
                               {"where", "wonderland"}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<input_t>::
        from_temporal_logic(
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
    using input_t = std::unordered_map<std::string, std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "1"}});
    sequence.push_back(input_t{{"time", "3"},
                               {"visitor1", "alice"},
                               {"visitor2", "charlotte"},
                               {"visiting1", "alice"},
                               {"where", "wonderland"}});
    sequence.push_back(input_t{{"time", "9"},
                               {"visiting1", "alice"},
                               {"visiting2", "charlotte"},
                               {"where", "wonderland"}});
    sequence.push_back(input_t{{"time", "13"},
                               {"visiting1", "alice"},
                               {"visiting2", "charlotte"},
                               {"where", "wonderland"}});
    sequence.push_back(input_t{{"time", "21"},
                               {"visiting1", "alice"},
                               {"visiting2", "charlotte"},
                               {"where", "wonderland"}});
    sequence.push_back(input_t{
        {"time", "23"}, {"visiting1", "charlotte"}, {"where", "wonderland"}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::dense_timed<time_t>::unordered_data::monitor<input_t>::
        from_temporal_logic(
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
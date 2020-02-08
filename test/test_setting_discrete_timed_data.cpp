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
// #include "reelay/data_manager.hpp"
#include "reelay/monitors.hpp"

using time_t = int64_t;
using value_t = BDD;

// TEST_CASE("Algebra of BDD Signals")
// {

//   /*
//    * Test BDD Signal Functionality
//    *
//    * BDD signals are functions from time domain to BDD instances.
//    * BDD signals are represented as boost::icl::interval_map<time_t, BDD>
//    * BDDs are managed by a particular BDD manager (see CUDD docs)
//    */

//   SECTION("Join")
//   {

//     auto &manager = reelay::data_manager::get();

//     auto map1 = reelay::data_interval_map<time_t>();
//     map1.add(std::make_pair(reelay::interval<time_t>::left_open(2, 5),
//                             manager.one()));

//     auto map2 = reelay::data_interval_map<time_t>();
//     map2.add(std::make_pair(reelay::interval<time_t>::left_open(4, 7),
//                             manager.one()));

//     auto result = map1 + map2;

//     auto expected = reelay::data_interval_map<time_t>();
//     expected.add(std::make_pair(reelay::interval<time_t>::left_open(2, 7),
//                                 manager.one()));

//     CHECK(result == expected);
//   }

//   SECTION("Meet1")
//   {

//     auto &manager = reelay::data_manager::get();

//     auto map1 = reelay::data_interval_map<time_t>();
//     map1.add(std::make_pair(reelay::interval<time_t>::left_open(0, 2),
//                             manager.zero()));
//     map1.add(std::make_pair(reelay::interval<time_t>::left_open(2, 5),
//                             manager.one()));
//     map1.add(std::make_pair(reelay::interval<time_t>::left_open(5, 9),
//                             manager.zero()));

//     auto map2 = reelay::data_interval_map<time_t>();
//     map2.add(std::make_pair(reelay::interval<time_t>::left_open(0, 4),
//                             manager.zero()));
//     map2.add(std::make_pair(reelay::interval<time_t>::left_open(4, 7),
//                             manager.one()));
//     map2.add(std::make_pair(reelay::interval<time_t>::left_open(7, 9),
//                             manager.zero()));

//     auto result = map1 - map2;

//     auto expected = reelay::data_interval_map<time_t>();
//     expected.add(std::make_pair(reelay::interval<time_t>::left_open(0, 4),
//                                 manager.zero()));
//     expected.add(std::make_pair(reelay::interval<time_t>::left_open(4, 5),
//                                 manager.one()));
//     expected.add(std::make_pair(reelay::interval<time_t>::left_open(5, 9),
//                                 manager.zero()));

//     CHECK(result == expected);
//   }

//   SECTION("Meet1")
//   {

//     auto &manager = reelay::data_manager::get();

//     auto map1 = reelay::data_interval_map<time_t>();
//     map1.add(std::make_pair(reelay::interval<time_t>::left_open(0, 2),
//                             manager.zero()));
//     map1.add(std::make_pair(reelay::interval<time_t>::left_open(2, 3),
//                             manager.one()));
//     map1.add(std::make_pair(reelay::interval<time_t>::left_open(3, 9),
//                             manager.zero()));

//     auto map2 = reelay::data_interval_map<time_t>();
//     map2.add(std::make_pair(reelay::interval<time_t>::left_open(0, 4),
//                             manager.zero()));
//     map2.add(std::make_pair(reelay::interval<time_t>::left_open(4, 7),
//                             manager.one()));
//     map2.add(std::make_pair(reelay::interval<time_t>::left_open(7, 9),
//                             manager.zero()));

//     auto result = map1 - map2;

//     auto expected = reelay::data_interval_map<time_t>();
//     expected.add(std::make_pair(reelay::interval<time_t>::left_open(0, 9),
//                                 manager.zero()));

//     CHECK(result == expected);
//   }

//   SECTION("Meet1")
//   {

//     auto &manager = reelay::data_manager::get();

//     auto map1 = reelay::data_interval_map<time_t>();
//     map1.add(std::make_pair(reelay::interval<time_t>::left_open(0, 2),
//                             manager.zero()));
//     map1.add(std::make_pair(reelay::interval<time_t>::left_open(2, 8),
//                             manager.one()));
//     map1.add(std::make_pair(reelay::interval<time_t>::left_open(8, 9),
//                             manager.zero()));

//     auto map2 = reelay::data_interval_map<time_t>();
//     map2.add(std::make_pair(reelay::interval<time_t>::left_open(0, 4),
//                             manager.zero()));
//     map2.add(std::make_pair(reelay::interval<time_t>::left_open(4, 7),
//                             manager.one()));
//     map2.add(std::make_pair(reelay::interval<time_t>::left_open(7, 9),
//                             manager.zero()));

//     auto result = map1 - map2;

//     auto expected = reelay::data_interval_map<time_t>();
//     expected.add(std::make_pair(reelay::interval<time_t>::left_open(0, 4),
//                                 manager.zero()));
//     expected.add(std::make_pair(reelay::interval<time_t>::left_open(4, 7),
//                                 manager.one()));
//     expected.add(std::make_pair(reelay::interval<time_t>::left_open(7, 9),
//                                 manager.zero()));

//     CHECK(result == expected);
//   }
// }

TEST_CASE("Atoms") {

  SECTION("Simple Proposition 1") {

    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "alice"});
    sequence.push_back(input_t{"logout", "bob"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("alice", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, f, t, t, f});
    CHECK(result == expected);
  }

  SECTION("Simple Proposition 2") {

    using input_t = std::unordered_map<std::string, std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{
        {"event", "access"}, {"user", "alice"}, {"file", "wonderland"}});
    sequence.push_back(input_t{
        {"event", "access"}, {"user", "bob"}, {"file", "feed_your_head"}});
    sequence.push_back(input_t{
        {"event", "access"}, {"user", "charlotte"}, {"file", "wonderland"}});
    sequence.push_back(input_t{{"event", "access"}, {"user", "charlotte"}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("user and file", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, t, t, f});
    CHECK(result == expected);
  }

  SECTION("List Proposition 0") {

    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "alice"});
    sequence.push_back(input_t{"tritiny", "bob", "humpie", "dumpie"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("[access, bob, white_rabbit]",
                                      extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, t, f, f, f});
    CHECK(result == expected);
  }

  SECTION("List Proposition 1") {

    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "alice"});
    sequence.push_back(input_t{"tritiny", "bob", "humpie", "dumpie"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("[access, alice]", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, f, f, f, f});
    CHECK(result == expected);
  }

  SECTION("List Proposition 2") {

    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "alice"});
    sequence.push_back(input_t{"tritiny", "bob", "humpie", "dumpie"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("[access, alice, *]", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, f, t, f, f});
    CHECK(result == expected);
  }

  SECTION("List Proposition 3") {

    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "alice"});
    sequence.push_back(input_t{"meet", "bob", "humpty", "dumpty"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("[*, alice, *]", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, f, t, f, f});
    CHECK(result == expected);
  }

  SECTION("List Proposition 4") {

    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "alice"});
    sequence.push_back(input_t{"meet", "bob", "humpty", "dumpty"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("[*event, alice, *]", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto datum = manager->assign("event", "access");

    auto expected = std::vector<reelay::data_set_t>({datum, f, datum, f, f});
    CHECK(result == expected);
  }

  SECTION("List Proposition 5") {

    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "alice"});
    sequence.push_back(input_t{"meet", "bob", "humpty", "dumpty"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("[access, *user, *file]", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto alice_in_wonderland = manager->assign("user", "alice") *
                               manager->assign("file", "wonderland");

    auto bobs_white_rabbit = manager->assign("user", "bob") *
                             manager->assign("file", "white_rabbit");

    auto alice_feed_your_head = manager->assign("user", "alice") *
                                manager->assign("file", "feed_your_head");

    auto expected = std::vector<reelay::data_set_t>(
        {alice_in_wonderland, bobs_white_rabbit, alice_feed_your_head, f, f});

    CHECK(result == expected);
  }

  SECTION("Record Proposition 0") {

    using input_t = std::unordered_map<std::string, std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{
        {"event", "access"}, {"user", "alice"}, {"file", "wonderland"}});
    sequence.push_back(input_t{
        {"event", "access"}, {"user", "alice"}, {"file", "feed_your_head"}});
    sequence.push_back(input_t{{"event", "access"},
                               {"user", "alice"},
                               {"file", "wonderland"},
                               {"version", "v2"}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::
        monitor<input_t>::from_temporal_logic(
            "{event: access, user: alice, file: wonderland}", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, f, t});
    CHECK(result == expected);
  }

  SECTION("Record Proposition 1") {

    using input_t = std::unordered_map<std::string, std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{
        {"event", "access"}, {"user", "alice"}, {"file", "wonderland"}});
    sequence.push_back(input_t{
        {"event", "access"}, {"user", "bob"}, {"file", "feed_your_head"}});
    sequence.push_back(input_t{
        {"event", "access"}, {"user", "charlotte"}, {"file", "wonderland"}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::
        monitor<input_t>::from_temporal_logic(
            "{event: access, user: *name, file: wonderland}", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto d1 = manager->assign("name", "alice");
    auto d3 = manager->assign("name", "charlotte");

    auto expected = std::vector<reelay::data_set_t>({d1, f, d3});
    CHECK(result == expected);
  }

  SECTION("Record Proposition 1") {

    using input_t = std::unordered_map<std::string, std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{
        {"event", "access"}, {"user", "alice"}, {"file", "wonderland"}});
    sequence.push_back(input_t{
        {"event", "access"}, {"user", "bob"}, {"file", "feed_your_head"}});
    sequence.push_back(input_t{
        {"event", "access"}, {"user", "charlotte"}, {"file", "wonderland"}});
    sequence.push_back(input_t{{"event", "access"}, {"user", "charlotte"}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("{event: access, user: *name, file:*}",
                                      extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto d1 = manager->assign("name", "alice");
    auto d2 = manager->assign("name", "bob");
    auto d3 = manager->assign("name", "charlotte");

    auto expected = std::vector<reelay::data_set_t>({d1, d2, d3, f});
    CHECK(result == expected);
  }
}

TEST_CASE("Boolean Operations") {

  SECTION("Negation") {

    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "alice"});
    sequence.push_back(input_t{"meet", "bob", "humpty", "dumpty"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("![access, alice, *]", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, t, f, t, t});

    CHECK(result == expected);
  }

  SECTION("Conjunction") {

    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "alice"});
    sequence.push_back(input_t{"meet", "bob", "humpty", "dumpty"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::
        monitor<input_t>::from_temporal_logic(
            "[*event, alice, *file] and [access, *user, wonderland]",
            extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto alice_in_wonderland = manager->assign("event", "access") *
                               manager->assign("user", "alice") *
                               manager->assign("file", "wonderland");

    auto expected =
        std::vector<reelay::data_set_t>({alice_in_wonderland, f, f, f, f});

    CHECK(result == expected);
  }

  SECTION("Disjunction") {

    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "alice"});
    sequence.push_back(input_t{"meet", "bob", "humpty", "dumpty"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::
        monitor<input_t>::from_temporal_logic(
            "[*event, alice, *file] or [access, *user, wonderland]",
            extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto datum1 = (manager->assign("event", "access") *
                   manager->assign("file", "wonderland")) +
                  manager->assign("user", "alice");

    auto datum3 = manager->assign("event", "access") *
                  manager->assign("file", "feed_your_head");

    auto expected = std::vector<reelay::data_set_t>({datum1, f, datum3, f, f});

    CHECK(result == expected);
  }
}

TEST_CASE("Untimed Temporal Operations") {

  SECTION("Previous") {

    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"open", "1"});
    sequence.push_back(input_t{"open", "2"});
    sequence.push_back(input_t{"close", "2"});
    sequence.push_back(input_t{"close", "2"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("pre([open, *])", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, t, t, f});

    CHECK(result == expected);
  }

  SECTION("Once") {
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"open", "a"});
    sequence.push_back(input_t{"open", "c"});
    sequence.push_back(input_t{"close", "c"});
    sequence.push_back(input_t{"close", "a"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("once([open, c])", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, t, t, t});

    CHECK(result == expected);
  }

  SECTION("Historically") {
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"close", "a"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("historically([is_open, a])", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, t, t, f});

    CHECK(result == expected);
  }

  SECTION("Since") {
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "b"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("[is_open, a] since [open, a]",
                                      extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, t, t, t, f});

    CHECK(result == expected);
  }

  SECTION("Existential Quantification") {

    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "alice"});
    sequence.push_back(input_t{"meet", "bob", "humpty", "dumpty"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("exists[event]. [*event, alice, *file]",
                                      extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto datum1 = manager->assign("file", "wonderland");
    auto datum3 = manager->assign("file", "feed_your_head");

    auto expected = std::vector<reelay::data_set_t>({datum1, f, datum3, f, f});

    CHECK(result == expected);
  }
}

TEST_CASE("Timed Temporal Operations") {

  SECTION("Timed Once") {
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"open", "a"});
    sequence.push_back(input_t{"open", "c"});
    sequence.push_back(input_t{"close", "c"});
    sequence.push_back(input_t{"close", "a"});
    sequence.push_back(input_t{"close", "c"});
    sequence.push_back(input_t{"close", "a"});
    sequence.push_back(input_t{"close", "c"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("once[2:4]([open, c])", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, f, f, t, t, t, f});

    CHECK(result == expected);
  }

  SECTION("Timed Once") {
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"open", "a"});
    sequence.push_back(input_t{"open", "c"});
    sequence.push_back(input_t{"close", "c"});
    sequence.push_back(input_t{"close", "a"});
    sequence.push_back(input_t{"close", "c"});
    sequence.push_back(input_t{"close", "a"});
    sequence.push_back(input_t{"close", "c"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("once[:4]([open, c])", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, t, t, t, t, t, f});

    CHECK(result == expected);
  }

  SECTION("Timed Once") {
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"open", "a"});
    sequence.push_back(input_t{"open", "c"});
    sequence.push_back(input_t{"close", "c"});
    sequence.push_back(input_t{"close", "a"});
    sequence.push_back(input_t{"close", "c"});
    sequence.push_back(input_t{"close", "a"});
    sequence.push_back(input_t{"close", "c"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("once[2:]([open, c])", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, f, f, t, t, t, t});

    CHECK(result == expected);
  }

  SECTION("Timed Historically") {
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"is_open", "b"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"close", "a"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("historically[:4]([is_open, a])",
                                      extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, f, f, f, f, t, f});

    CHECK(result == expected);
  }

  SECTION("Timed Historically") {
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"is_open", "b"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"close", "a"});
    sequence.push_back(input_t{"close", "a"});
    sequence.push_back(input_t{"close", "a"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("historically[2:4]([is_open, a])",
                                      extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected =
        std::vector<reelay::data_set_t>({f, f, f, f, f, t, t, t, f});

    CHECK(result == expected);
  }

  SECTION("Timed Historically") {
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"close", "a"});
    sequence.push_back(input_t{"close", "a"});
    sequence.push_back(input_t{"close", "a"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("historically[2:]([is_open, a])",
                                      extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, t, t, t, t, t, t, f});

    CHECK(result == expected);
  }

  SECTION("Timed Since") {
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "b"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("[is_open, a] since[2:4] [open, a]",
                                      extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, f, t, t, t, f, f, f});

    CHECK(result == expected);
  }

  SECTION("Timed Since") {
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "b"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("[is_open, a] since[:4] [open, a]",
                                      extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, t, t, t, t, f, f, f});

    CHECK(result == expected);
  }

  SECTION("Timed Since") {
    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "b"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::discrete_timed<time_t>::unordered_data::monitor<
        input_t>::from_temporal_logic("[is_open, a] since[2:] [open, a]",
                                      extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, f, t, t, t, t, t, f});

    CHECK(result == expected);
  }
}
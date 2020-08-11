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
#include "reelay/json.hpp"
//
#include "reelay/networks/discrete_timed_data_network.hpp"
#include "reelay/options.hpp"

using input_type = reelay::json;
using time_type = int64_t;

TEST_CASE("Atoms") {
  auto manager = std::make_shared<reelay::binding_manager>();
  auto opts = reelay::basic_options().with_data_manager(manager);

  SECTION("AtomicProposition") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x", true}});
    sequence.push_back(input_type{{"x", false}});
    sequence.push_back(input_type{});
    sequence.push_back(input_type{{"x", true}});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "{x}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, f, f, t});
    CHECK(result == expected);
  }

  SECTION("AtomicTrue") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x", true}});
    sequence.push_back(input_type{{"x", false}});
    sequence.push_back(input_type{});
    sequence.push_back(input_type{{"x", true}});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "{x: true}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, f, f, t});
    CHECK(result == expected);
  }

  SECTION("AtomicFalse") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x", true}});
    sequence.push_back(input_type{{"x", false}});
    sequence.push_back(input_type{});
    sequence.push_back(input_type{{"x", true}});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "{x: false}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, t, t, f});
    CHECK(result == expected);
  }

  SECTION("AtomicString") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x", "a"}});
    sequence.push_back(input_type{{"x", "b"}});
    sequence.push_back(input_type{});
    sequence.push_back(input_type{{"x", "c"}});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "{x: b}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, t, t, f});
    CHECK(result == expected);
  }

  SECTION("AtomicNumber") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x", 1}});
    sequence.push_back(input_type{{"x", 2}});
    sequence.push_back(input_type{});
    sequence.push_back(input_type{{"x", 3}});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "{x: 2}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, t, t, f});
    CHECK(result == expected);
  }

  SECTION("GreaterThan") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x", 1}});
    sequence.push_back(input_type{{"x", 2}});
    sequence.push_back(input_type{});
    sequence.push_back(input_type{{"x", 3}});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "{x > 2}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, f, f, t});
    CHECK(result == expected);
  }

  SECTION("GreaterEqual") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x", 1}});
    sequence.push_back(input_type{{"x", 2}});
    sequence.push_back(input_type{});
    sequence.push_back(input_type{{"x", 3}});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "{x >= 2}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, t, t, t});
    CHECK(result == expected);
  }

  SECTION("LessThan") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x", 1}});
    sequence.push_back(input_type{{"x", 2}});
    sequence.push_back(input_type{});
    sequence.push_back(input_type{{"x", 3}});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "{x < 2}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, f, f, f});
    CHECK(result == expected);
  }

  SECTION("LessEqual") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x", 1}});
    sequence.push_back(input_type{{"x", 2}});
    sequence.push_back(input_type{});
    sequence.push_back(input_type{{"x", 3}});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "{x <= 2}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, t, t, f});
    CHECK(result == expected);
  }

  SECTION("Equal") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x", 1}});
    sequence.push_back(input_type{{"x", 2}});
    sequence.push_back(input_type{});
    sequence.push_back(input_type{{"x", 3}});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "{x == 2}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, t, t, f});
    CHECK(result == expected);
  }

  SECTION("NotEqual") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x", 1}});
    sequence.push_back(input_type{{"x", 2}});
    sequence.push_back(input_type{});
    sequence.push_back(input_type{{"x", 3}});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "{x != 2}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, f, f, t});
    CHECK(result == expected);
  }

  SECTION("AtomicAny") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{{"x", "1"}});
    sequence.push_back(input_type{{"y", "2"}});
    sequence.push_back(input_type{{"x", "3"}});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "{x: *}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, f, t});
    CHECK(result == expected);
  }

  SECTION("List Proposition 0") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"access", "alice", "wonderland"});
    sequence.push_back(input_type{"access", "bob", "white_rabbit"});
    sequence.push_back(input_type{"access", "alice", "feed_your_head"});
    sequence.push_back(input_type{"logout", "alice"});
    sequence.push_back(input_type{"tritiny", "bob", "humpie", "dumpie"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "{$0: access, $1: bob, $2: white_rabbit}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, t, f, f, f});
    CHECK(result == expected);
  }

  SECTION("List Proposition 1") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"access", "alice", "wonderland"});
    sequence.push_back(input_type{"access", "bob", "white_rabbit"});
    sequence.push_back(input_type{"access", "alice", "feed_your_head"});
    sequence.push_back(input_type{"logout", "alice"});
    sequence.push_back(input_type{"tritiny", "bob", "humpie", "dumpie"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "{$0: access, $1: alice}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, f, t, f, f});
    CHECK(result == expected);
  }

  SECTION("List Proposition 3") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"access", "alice", "wonderland"});
    sequence.push_back(input_type{"access", "bob", "white_rabbit"});
    sequence.push_back(input_type{"access", "alice", "feed_your_head"});
    sequence.push_back(input_type{"logout", "alice"});
    sequence.push_back(input_type{"meet", "bob", "humpty", "dumpty"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "{$0: *, $1: alice, $2: *}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, f, t, f, f});
    CHECK(result == expected);
  }

  SECTION("List Proposition 4") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"access", "alice", "wonderland"});
    sequence.push_back(input_type{"access", "bob", "white_rabbit"});
    sequence.push_back(input_type{"access", "alice", "feed_your_head"});
    sequence.push_back(input_type{"logout", "charlotte"});
    sequence.push_back(input_type{"meet", "bob", "humpty", "dumpty"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "{$0: *event, $1: alice,  $2: *}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto datum = manager->assign("event", "access");

    auto expected = std::vector<reelay::data_set_t>({datum, f, datum, f, f});
    CHECK(result == expected);
  }

  SECTION("List Proposition 5") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"access", "alice", "wonderland"});
    sequence.push_back(input_type{"access", "bob", "white_rabbit"});
    sequence.push_back(input_type{"access", "alice", "feed_your_head"});
    sequence.push_back(input_type{"logout", "alice"});
    sequence.push_back(input_type{"meet", "bob", "humpty", "dumpty"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "{$0: access, $1: *user, $2: *file}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto alice_in_wonderland = manager->assign("user", "alice")
                               * manager->assign("file", "wonderland");

    auto bobs_white_rabbit = manager->assign("user", "bob")
                             * manager->assign("file", "white_rabbit");

    auto alice_feed_your_head = manager->assign("user", "alice")
                                * manager->assign("file", "feed_your_head");

    auto expected = std::vector<reelay::data_set_t>(
        {alice_in_wonderland, bobs_white_rabbit, alice_feed_your_head, f, f});

    CHECK(result == expected);
  }

  SECTION("Record Proposition 0") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{
        {"event", "access"}, {"user", "alice"}, {"file", "wonderland"}});
    sequence.push_back(input_type{
        {"event", "access"}, {"user", "alice"}, {"file", "feed_your_head"}});
    sequence.push_back(input_type{
        {"event", "access"},
        {"user", "alice"},
        {"file", "wonderland"},
        {"version", "v2"}});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
            "{event: access, user: alice, file: wonderland}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, f, t});
    CHECK(result == expected);
  }

  SECTION("Record Proposition 1") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{
        {"event", "access"}, {"user", "alice"}, {"file", "wonderland"}});
    sequence.push_back(input_type{
        {"event", "access"}, {"user", "bob"}, {"file", "feed_your_head"}});
    sequence.push_back(input_type{
        {"event", "access"}, {"user", "charlotte"}, {"file", "wonderland"}});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
            "{event: access, user: *name, file: wonderland}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto d1 = manager->assign("name", "alice");
    auto d3 = manager->assign("name", "charlotte");

    auto expected = std::vector<reelay::data_set_t>({d1, f, d3});
    CHECK(result == expected);
  }

  SECTION("Record Proposition 1") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{
        {"event", "access"}, {"user", "alice"}, {"file", "wonderland"}});
    sequence.push_back(input_type{
        {"event", "access"}, {"user", "bob"}, {"file", "feed_your_head"}});
    sequence.push_back(input_type{
        {"event", "access"}, {"user", "charlotte"}, {"file", "wonderland"}});
    sequence.push_back(input_type{{"event", "access"}, {"user", "charlotte"}});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "{event: access, user: *name, file:*}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
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
  auto manager = std::make_shared<reelay::binding_manager>();
  auto opts = reelay::basic_options().with_data_manager(manager);

  SECTION("Negation") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"access", "alice", "wonderland"});
    sequence.push_back(input_type{"access", "bob", "white_rabbit"});
    sequence.push_back(input_type{"access", "alice", "feed_your_head"});
    sequence.push_back(input_type{"logout", "alice"});
    sequence.push_back(input_type{"meet", "bob", "humpty", "dumpty"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "!{$0: access, $1: alice, $2: *}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, t, f, t, t});

    CHECK(result == expected);
  }

  SECTION("Conjunction") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"access", "alice", "wonderland"});
    sequence.push_back(input_type{"access", "bob", "white_rabbit"});
    sequence.push_back(input_type{"access", "alice", "feed_your_head"});
    sequence.push_back(input_type{"logout", "alice"});
    sequence.push_back(input_type{"meet", "bob", "humpty", "dumpty"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
            "{$0: *event, $1: alice, $2: *file} and "
            "{$0: access, $1: *user, $2: wonderland}",
            opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto alice_in_wonderland = manager->assign("event", "access")
                               * manager->assign("user", "alice")
                               * manager->assign("file", "wonderland");

    auto expected
        = std::vector<reelay::data_set_t>({alice_in_wonderland, f, f, f, f});

    CHECK(result == expected);
  }

  SECTION("Disjunction") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"access", "alice", "wonderland"});
    sequence.push_back(input_type{"access", "bob", "white_rabbit"});
    sequence.push_back(input_type{"access", "alice", "feed_your_head"});
    sequence.push_back(input_type{"logout", "charlotte"});
    sequence.push_back(input_type{"meet", "bob", "humpty", "dumpty"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
            "{$0: *event, $1: alice, $2: *file} or "
            "{$0: access, $1: *user, $2: wonderland}",
            opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto datum1 = (manager->assign("event", "access")
                   * manager->assign("file", "wonderland"))
                  + manager->assign("user", "alice");

    auto datum3 = manager->assign("event", "access")
                  * manager->assign("file", "feed_your_head");

    auto expected = std::vector<reelay::data_set_t>({datum1, f, datum3, f, f});

    CHECK(result == expected);
  }

  SECTION("Implication") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"access", "alice", "wonderland"});
    sequence.push_back(input_type{"access", "bob", "white_rabbit"});
    sequence.push_back(input_type{"access", "alice", "feed_your_head"});
    sequence.push_back(input_type{"logout", "charlotte"});
    sequence.push_back(input_type{"meet", "bob", "humpty", "dumpty"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
            "!{$0: *event, $1: alice, $2: *file} implies "
            "{$0: access, $1: *user, $2: wonderland}",
            opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto datum1 = (manager->assign("event", "access")
                   * manager->assign("file", "wonderland"))
                  + manager->assign("user", "alice");

    auto datum3 = manager->assign("event", "access")
                  * manager->assign("file", "feed_your_head");

    auto expected = std::vector<reelay::data_set_t>({datum1, f, datum3, f, f});

    CHECK(result == expected);
  }
}

TEST_CASE("Untimed Temporal Operations") {
  auto manager = std::make_shared<reelay::binding_manager>();
  auto opts = reelay::basic_options().with_data_manager(manager);

  SECTION("Previous") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"open", "1"});
    sequence.push_back(input_type{"open", "2"});
    sequence.push_back(input_type{"close", "2"});
    sequence.push_back(input_type{"close", "2"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "pre{$0: open,  $1: *}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, t, t, f});

    CHECK(result == expected);
  }

  SECTION("Once") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"open", "a"});
    sequence.push_back(input_type{"open", "c"});
    sequence.push_back(input_type{"close", "c"});
    sequence.push_back(input_type{"close", "a"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "once{$0: open, $1: c}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, t, t, t});

    CHECK(result == expected);
  }

  SECTION("Historically") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"close", "a"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "historically{$0: is_open, $1: a}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, t, t, f});

    CHECK(result == expected);
  }

  SECTION("Since") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "b"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
            "{$0: is_open, $1: a} since {$0: open, $1: a}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, t, t, t, f});

    CHECK(result == expected);
  }

  SECTION("Existential Quantification") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"access", "alice", "wonderland"});
    sequence.push_back(input_type{"access", "bob", "white_rabbit"});
    sequence.push_back(input_type{"access", "alice", "feed_your_head"});
    sequence.push_back(input_type{"logout", "charlotte"});
    sequence.push_back(input_type{"meet", "bob", "humpty", "dumpty"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
            "exists[event]. {$0: *event, $1: alice, $2: *file}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto datum1 = manager->assign("file", "wonderland");
    auto datum3 = manager->assign("file", "feed_your_head");

    auto expected = std::vector<reelay::data_set_t>({datum1, f, datum3, f, f});

    CHECK(result == expected);
  }

  SECTION("Universal Quantification") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"access", "alice", "wonderland"});
    sequence.push_back(input_type{"access", "bob", "white_rabbit"});
    sequence.push_back(input_type{"access", "alice", "feed_your_head"});
    sequence.push_back(input_type{"logout", "charlotte"});
    sequence.push_back(input_type{"meet", "bob", "humpty", "dumpty"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
            "not(forall[event].(!{$0: *event, $1: alice, $2: *file}))", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
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
  auto manager = std::make_shared<reelay::binding_manager>();
  auto opts = reelay::basic_options().with_data_manager(manager);
  
  SECTION("Timed Once") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"open", "a"});
    sequence.push_back(input_type{"open", "c"});
    sequence.push_back(input_type{"close", "c"});
    sequence.push_back(input_type{"close", "a"});
    sequence.push_back(input_type{"close", "c"});
    sequence.push_back(input_type{"close", "a"});
    sequence.push_back(input_type{"close", "c"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "once[2:4]{$0: open, $1: c}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, f, f, t, t, t, f});

    CHECK(result == expected);
  }

  SECTION("Timed Once") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"open", "a"});
    sequence.push_back(input_type{"open", "c"});
    sequence.push_back(input_type{"close", "c"});
    sequence.push_back(input_type{"close", "a"});
    sequence.push_back(input_type{"close", "c"});
    sequence.push_back(input_type{"close", "a"});
    sequence.push_back(input_type{"close", "c"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "once[:4]{$0: open, $1: c}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, t, t, t, t, t, f});

    CHECK(result == expected);
  }

  SECTION("Timed Once") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"open", "a"});
    sequence.push_back(input_type{"open", "c"});
    sequence.push_back(input_type{"close", "c"});
    sequence.push_back(input_type{"close", "a"});
    sequence.push_back(input_type{"close", "c"});
    sequence.push_back(input_type{"close", "a"});
    sequence.push_back(input_type{"close", "c"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "once[2:]{$0: open, $1: c}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, f, f, t, t, t, t});

    CHECK(result == expected);
  }

  SECTION("Timed Historically") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"is_open", "b"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"close", "a"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "historically[:4]{$0:is_open, $1:a}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, f, f, f, f, t, f});

    CHECK(result == expected);
  }

  SECTION("Timed Historically") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"is_open", "b"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"close", "a"});
    sequence.push_back(input_type{"close", "a"});
    sequence.push_back(input_type{"close", "a"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "historically[2:4]{$0:is_open, $1:a}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected
        = std::vector<reelay::data_set_t>({f, f, f, f, f, t, t, t, f});

    CHECK(result == expected);
  }

  SECTION("Timed Historically") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"close", "a"});
    sequence.push_back(input_type{"close", "a"});
    sequence.push_back(input_type{"close", "a"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
        "historically[2:]{$0:is_open, $1:a}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, t, t, t, t, t, t, f});

    CHECK(result == expected);
  }

  SECTION("Timed Since") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "b"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
            "{$0:is_open, $1:a} since[2:4] {$0: open, $1: a}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, f, t, t, t, f, f, f});

    CHECK(result == expected);
  }

  SECTION("Timed Since") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "b"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
            "{$0:is_open, $1:a} since[:4] {$0: open, $1: a}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, t, t, t, t, f, f, f});

    CHECK(result == expected);
  }

  SECTION("Timed Since") {
    std::vector<input_type> sequence = std::vector<input_type>();

    sequence.push_back(input_type{"open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "a"});
    sequence.push_back(input_type{"is_open", "b"});

    auto net1 = reelay::discrete_timed_data_network<time_type, input_type>::make(
            "{$0:is_open, $1:a} since[2:] {$0: open, $1: a}", opts);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1.update(row);
      result.push_back(net1.output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, f, t, t, t, t, t, f});

    CHECK(result == expected);
  }
}
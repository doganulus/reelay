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
#include "reelay/json.hpp"

using input_t = reelay::json;
// using input_t = std::vector<std::vector<std::string>>;
// using input_t = std::vector<std::unordered_map<std::string,
// std::string>>;

TEST_CASE("Atoms") {

  SECTION("List Proposition 0") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "alice"});
    sequence.push_back(input_t{"tritiny", "bob", "humpie", "dumpie"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "{$0: access, $1: bob, $2: white_rabbit}", extra_args);

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

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "alice"});
    sequence.push_back(input_t{"tritiny", "bob", "humpie", "dumpie"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "{$0: access, $1: alice, $2: *}", extra_args);

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

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "alice"});
    sequence.push_back(input_t{"meet", "bob", "humpty", "dumpty"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "{$0: *, $1: alice, $2: *}", extra_args);

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

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "alice"});
    sequence.push_back(input_t{"meet", "bob", "humpty", "dumpty"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "{$0: *event, $1: alice, $2: *}", extra_args);

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

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "alice"});
    sequence.push_back(input_t{"meet", "bob", "humpty", "dumpty"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "{$0: access, $1: *user, $2: *file}", extra_args);

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

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x", true}});
    sequence.push_back(input_t{{"x", false}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x", true}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "{x}", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, f, f, t});
    CHECK(result == expected);
  }

  SECTION("AtomicTrue") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x", true}});
    sequence.push_back(input_t{{"x", false}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x", true}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "{x: true}", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, f, f, t});
    CHECK(result == expected);
  }

  SECTION("AtomicFalse") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x", true}});
    sequence.push_back(input_t{{"x", false}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x", true}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "{x: false}", extra_args);

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

  SECTION("AtomicString") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x", "a"}});
    sequence.push_back(input_t{{"x", "b"}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x", "c"}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "{x: b}", extra_args);

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

  SECTION("AtomicNumber") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x", 1}});
    sequence.push_back(input_t{{"x", 2}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x", 3}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "{x: 2}", extra_args);

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

  SECTION("GreaterThan") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x", 1}});
    sequence.push_back(input_t{{"x", 2}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x", 3}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "{x > 2}", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, f, f, t});
    CHECK(result == expected);
  }

  SECTION("GreaterEqual") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x", 1}});
    sequence.push_back(input_t{{"x", 2}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x", 3}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "{x >= 2}", extra_args);

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

  SECTION("LessThan") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x", 1}});
    sequence.push_back(input_t{{"x", 2}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x", 3}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "{x < 2}", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, f, f, f});
    CHECK(result == expected);
  }

  SECTION("LessEqual") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x", 1}});
    sequence.push_back(input_t{{"x", 2}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x", 3}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "{x <= 2}", extra_args);

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

  SECTION("Equal") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x", 1}});
    sequence.push_back(input_t{{"x", 2}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x", 3}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "{x == 2}", extra_args);

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

  SECTION("NotEqual") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x", 1}});
    sequence.push_back(input_t{{"x", 2}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"x", 3}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "{x != 2}", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, f, f, t});
    CHECK(result == expected);
  }

  SECTION("AtomicAny") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"x", "1"}});
    sequence.push_back(input_t{{"y", "2"}});
    sequence.push_back(input_t{{"x", "3"}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "{x: *}", extra_args);

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

  SECTION("Record Proposition 0") {

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

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
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

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{
        {"event", "access"}, {"user", "alice"}, {"file", "wonderland"}});
    sequence.push_back(input_t{
        {"event", "access"}, {"user", "bob"}, {"file", "feed_your_head"}});
    sequence.push_back(input_t{
        {"event", "access"}, {"user", "charlotte"}, {"file", "wonderland"}});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
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

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "{event: access, user: *name, file:*}", extra_args);

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

TEST_CASE("Nested Inputs") {

  SECTION("Deep Object") {
    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"obj", {{"flag", false}}}});
    sequence.push_back(input_t{{"obj", {{"flag", false}}}});
    sequence.push_back(input_t{});
    sequence.push_back(input_t{{"obj", {{"flag", true}}}});
    sequence.push_back(input_t{});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "obj::{flag}", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, f, f, t, t});

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

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "obj1::obj2::{flag2}", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({f, t, t, f, f});

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

  //   auto manager = std::make_shared<reelay::binding_manager>();
  //   reelay::kwargs extra_args = {{"manager", manager}};

  //   auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
  //       "obj1::obj2::any{a < 6}", extra_args);

  //   auto result = std::vector<reelay::data_set_t>();

  //   for (const auto &row : sequence) {
  //     net1->update(row);
  //     result.push_back(net1->output());
  //   }

  //   auto t = manager->one();
  //   auto f = manager->zero();

  //   auto expected = std::vector<reelay::data_set_t>({f, t, t});

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

  //   auto manager = std::make_shared<reelay::binding_manager>();
  //   reelay::kwargs extra_args = {{"manager", manager}};

  //   auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
  //       "obj1::obj2::all{b > 1}", extra_args);

  //   auto result = std::vector<reelay::data_set_t>();

  //   for (const auto &row : sequence) {
  //     net1->update(row);
  //     result.push_back(net1->output());
  //   }

  //   auto t = manager->one();
  //   auto f = manager->zero();

  //   auto expected = std::vector<reelay::data_set_t>({t, t, f});

  //   CHECK(result == expected);
  // }
}

TEST_CASE("Boolean Operations") {

  SECTION("Negation") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "alice"});
    sequence.push_back(input_t{"meet", "bob", "humpty", "dumpty"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "!{$0: access, $1: alice}", extra_args);

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

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "alice"});
    sequence.push_back(input_t{"meet", "bob", "humpty", "dumpty"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "{$0: *event, $1: alice, $2: *file} and {$0: access, $1: *user, "
        "$2: wonderland}",
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

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "alice"});
    sequence.push_back(input_t{"meet", "bob", "humpty", "dumpty"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "{$0: access, $1 :alice, $2: *file} or {$0: access, $1: *user, "
        "$2: wonderland}",
        extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto datum1 = manager->assign("file", "wonderland") +
                  manager->assign("user", "alice");

    auto datum3 = manager->assign("file", "feed_your_head");

    auto expected = std::vector<reelay::data_set_t>({datum1, f, datum3, f, f});

    CHECK(result == expected);
  }
}

TEST_CASE("Quantification") {

  SECTION("Existence") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "charlotte"});
    sequence.push_back(input_t{"meet", "bob", "humpty", "dumpty"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "exists[event]. {$0: *event, $1: alice, $2: *file}", extra_args);

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

TEST_CASE("Temporal Operations") {

  SECTION("Previous") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"open", "1"});
    sequence.push_back(input_t{"open", "2"});
    sequence.push_back(input_t{"close", "2"});
    sequence.push_back(input_t{"close", "2"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "pre({$0: open, $1: *})", extra_args);

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

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"open", "a"});
    sequence.push_back(input_t{"open", "c"});
    sequence.push_back(input_t{"close", "c"});
    sequence.push_back(input_t{"close", "a"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "once{$0: open, $1: c}", extra_args);

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

  SECTION("Once") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"open", "a"});
    sequence.push_back(input_t{"open", "c"});
    sequence.push_back(input_t{"close", "c"});
    sequence.push_back(input_t{"close", "a"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "once{$0: open, $1: *file}", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto file_a = manager->assign("file", "a");
    auto file_c = manager->assign("file", "c");

    auto expected = std::vector<reelay::data_set_t>({
        file_a,
        file_a + file_c,
        file_a + file_c,
        file_a + file_c,
    });

    CHECK(result == expected);
  }

  SECTION("Historically") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"close", "a"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "historically{$0: is_open, $1: a}", extra_args);

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

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "a"});
    sequence.push_back(input_t{"is_open", "b"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "{$0: is_open, $1: a} since {$0: open, $1: a}", extra_args);

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

}

TEST_CASE("Some complex formulas") {

  SECTION("Attempt to access a closed file") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"login", "alice"});
    sequence.push_back(input_t{"login", "bob"});
    sequence.push_back(input_t{"open", "wonderland"});
    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"close", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "wonderland"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "forall[user,file]. {$0: access, $1: *user, $2: *file} implies "
        "(!{$0: logout, $1: *user} since {$0: login, $1: *user} and !{$0: "
        "close, $1: *file} since {$0: open, $1: *file})",
        extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>({t, t, t, t, t, f});

    CHECK(result == expected);
  }

  SECTION("Neither Open Nor Close") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"open", "1", "read"});
    sequence.push_back(input_t{"open", "2", "read"});
    sequence.push_back(input_t{"open", "3", "read"});
    sequence.push_back(input_t{"open", "4", "read"});
    sequence.push_back(input_t{"close", "4"});
    sequence.push_back(input_t{"open", "3", "write"});
    sequence.push_back(input_t{"open", "4", "write"});
    sequence.push_back(input_t{"close", "4"});
    sequence.push_back(input_t{"close", "3"});
    sequence.push_back(input_t{"close", "2"});
    sequence.push_back(input_t{"close", "1"});
    sequence.push_back(input_t{"close", "4"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "forall[file]. {$0: close, $1: *file} implies (exists[mode]. "
        "pre(!{$0: close, $1: *file} since {$0: open, $1: *file, $2: *mode}))",
        extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected =
        std::vector<reelay::data_set_t>({t, t, t, t, t, t, t, t, t, t, t, f});

    CHECK(result == expected);
  }

  SECTION("First In-n-Out") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"enter", "a"});
    sequence.push_back(input_t{"enter", "b"});
    sequence.push_back(input_t{"enter", "c"});
    sequence.push_back(input_t{"enter", "d"});
    sequence.push_back(input_t{"enter", "e"});
    sequence.push_back(input_t{"enter", "f"});
    sequence.push_back(input_t{"enter", "g"});
    sequence.push_back(input_t{"enter", "h"});
    sequence.push_back(input_t{"exit", "a"});
    sequence.push_back(input_t{"exit", "b"});
    sequence.push_back(input_t{"exit", "d"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "forall[x,y].({$0: exit, $1: *y} and once({$0: enter, $1: *y} and "
        "pre(once{$0: enter, $1: *x})) implies pre(once{$0: exit, $1: *x}))",
        extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected =
        std::vector<reelay::data_set_t>({t, t, t, t, t, t, t, t, t, t, f});

    CHECK(result == expected);
  }
}
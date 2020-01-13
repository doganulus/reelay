#include "iostream"
#include "vector"

#include "catch.hpp"

#include "reelay/common.hpp"
#include "reelay/monitors.hpp"

// using input_t = std::vector<std::vector<std::string>>;
// using input_t = std::vector<std::unordered_map<std::string, std::string>>;

TEST_CASE( "Atoms" ) {

  SECTION("Simple Proposition") {

    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "alice"});
    sequence.push_back(input_t{"logout", "bob"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "alice", extra_args);

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

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "[access, bob, white_rabbit]", extra_args);

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

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "[access, alice]", extra_args);

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

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "[access, alice, *]", extra_args);

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

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "[*, alice, *]", extra_args);

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

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "[*event, alice, *]", extra_args);

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

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "[access, *user, *file]", extra_args);

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

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "![access, alice, *]", extra_args);

    auto result = std::vector<reelay::data_set_t>();

    for (const auto &row : sequence) {
      net1->update(row);
      result.push_back(net1->output());
    }

    auto t = manager->one();
    auto f = manager->zero();

    auto expected = std::vector<reelay::data_set_t>(
        {f, t, f, t, t});

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

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "[*event, alice, *file] and [access, *user, wonderland]", extra_args);

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

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "[*event, alice, *file] or [access, *user, wonderland]", extra_args);

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

TEST_CASE("Quantification") {

  SECTION("Existence") {

    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "alice"});
    sequence.push_back(input_t{"meet", "bob", "humpty", "dumpty"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "exists[event]. [*event, alice, *file]", extra_args);

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

  SECTION("Existence") {

    using input_t = std::vector<std::string>;

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{"access", "alice", "wonderland"});
    sequence.push_back(input_t{"access", "bob", "white_rabbit"});
    sequence.push_back(input_t{"access", "alice", "feed_your_head"});
    sequence.push_back(input_t{"logout", "alice"});
    sequence.push_back(input_t{"meet", "bob", "humpty", "dumpty"});

    auto manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs extra_args = {{"manager", manager}};

    auto net1 = reelay::unordered_data::monitor<input_t>::from_temporal_logic(
        "exists[event]. [*event, alice, *file]", extra_args);

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
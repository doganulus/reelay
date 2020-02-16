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
#include "reelay/intervals.hpp"
#include "reelay/monitors.hpp"

using input_t = std::unordered_map<std::string, std::string>;
using value_t = int64_t;
using dense_time_t = int64_t;

using interval = reelay::interval<dense_time_t>;
using robustness_map = reelay::robustness_interval_map<dense_time_t, value_t>;

value_t top = reelay::infinity<value_t>::value();
value_t bot = -reelay::infinity<value_t>::value();

TEST_CASE("Atoms") {

  SECTION("Proposition") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "10"}, {"x", "11"}});
    sequence.push_back(input_t{{"time", "20"}, {"x", "15"}});
    sequence.push_back(input_t{{"time", "42"}, {"x", "13"}});

    auto net1 = reelay::dense_timed<dense_time_t>::robustness<value_t>::monitor<
        input_t>::from_temporal_logic("{x>0}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1->update(s);
      result1 = result1 | net1->output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 10), 11));
    expected1.add(std::make_pair(interval::left_open(10, 20), 15));
    expected1.add(std::make_pair(interval::left_open(20, 42), 13));

    CHECK(result1 == expected1);
  }

  SECTION("GreaterEqualZeroOrder") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "1"}, {"x1", "3"}});
    sequence.push_back(input_t{{"time", "6"}, {"x1", "5"}});
    sequence.push_back(input_t{{"time", "9"}, {"x1", "4"}});

    auto net1 = reelay::dense_timed<dense_time_t>::robustness<value_t>::monitor<
        input_t>::from_temporal_logic("{x1 >= 4}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1->update(s);
      result1 = result1 | net1->output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 1), -1));
    expected1.add(std::make_pair(interval::left_open(1, 6), 1));
    expected1.add(std::make_pair(interval::left_open(6, 9), 0));

    CHECK(result1 == expected1);
  }

  SECTION("GreaterThanZeroOrder") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "1"}, {"x1", "3"}});
    sequence.push_back(input_t{{"time", "6"}, {"x1", "5"}});
    sequence.push_back(input_t{{"time", "9"}, {"x1", "4"}});

    auto net1 = reelay::dense_timed<dense_time_t>::robustness<value_t>::monitor<
        input_t>::from_temporal_logic("{x1 > 4}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1->update(s);
      result1 = result1 | net1->output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 1), -1));
    expected1.add(std::make_pair(interval::left_open(1, 6), 1));
    expected1.add(std::make_pair(interval::left_open(6, 9), 0));

    CHECK(result1 == expected1);
  }

  SECTION("LessEqualZeroOrder") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "1"}, {"x1", "3"}});
    sequence.push_back(input_t{{"time", "6"}, {"x1", "5"}});
    sequence.push_back(input_t{{"time", "9"}, {"x1", "4"}});

    auto net1 = reelay::dense_timed<dense_time_t>::robustness<value_t>::monitor<
        input_t>::from_temporal_logic("{x1 <= 4}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1->update(s);
      result1 = result1 | net1->output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 1), 1));
    expected1.add(std::make_pair(interval::left_open(1, 6), -1));
    expected1.add(std::make_pair(interval::left_open(6, 9), 0));

    CHECK(result1 == expected1);
  }

  SECTION("LessThanZeroOrder") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "1"}, {"x1", "3"}});
    sequence.push_back(input_t{{"time", "6"}, {"x1", "5"}});
    sequence.push_back(input_t{{"time", "9"}, {"x1", "4"}});

    auto net1 = reelay::dense_timed<dense_time_t>::robustness<value_t>::monitor<
        input_t>::from_temporal_logic("{x1 <= 4}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1->update(s);
      result1 = result1 | net1->output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 1), 1));
    expected1.add(std::make_pair(interval::left_open(1, 6), -1));
    expected1.add(std::make_pair(interval::left_open(6, 9), 0));

    CHECK(result1 == expected1);
  }
}

TEST_CASE("Boolean Operations") {

  SECTION("Negation") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "10"}, {"x", "11"}});
    sequence.push_back(input_t{{"time", "20"}, {"x", "15"}});

    auto net1 = reelay::dense_timed<dense_time_t>::robustness<value_t>::monitor<
        input_t>::from_temporal_logic("not{x >= 12}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1->update(s);
      result1 = result1 | net1->output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 10), 1));
    expected1.add(std::make_pair(interval::left_open(10, 20), -3));

    CHECK(result1 == expected1);
  }

  SECTION("Disjunction") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "10"}, {"x", "11"}, {"y", "10"}});
    sequence.push_back(input_t{{"time", "20"}, {"x", "15"}, {"y", "12"}});
    sequence.push_back(input_t{{"time", "42"}, {"x", "13"}, {"y", "17"}});

    auto net1 = reelay::dense_timed<dense_time_t>::robustness<value_t>::monitor<
        input_t>::from_temporal_logic("{x>0} or {y>0}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1->update(s);
      result1 = result1 | net1->output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 10), 11));
    expected1.add(std::make_pair(interval::left_open(10, 20), 15));
    expected1.add(std::make_pair(interval::left_open(20, 42), 17));

    CHECK(result1 == expected1);
  }

  SECTION("Conjunction") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "10"}, {"x", "11"}, {"y", "10"}});
    sequence.push_back(input_t{{"time", "20"}, {"x", "15"}, {"y", "12"}});
    sequence.push_back(input_t{{"time", "42"}, {"x", "13"}, {"y", "17"}});

    auto net1 = reelay::dense_timed<dense_time_t>::robustness<value_t>::monitor<
        input_t>::from_temporal_logic("{x>0} and {y>0}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1->update(s);
      result1 = result1 | net1->output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 10), 10));
    expected1.add(std::make_pair(interval::left_open(10, 20), 12));
    expected1.add(std::make_pair(interval::left_open(20, 42), 13));

    CHECK(result1 == expected1);
  }

  SECTION("Implication") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "10"}, {"x", "-11"}, {"y", "10"}});
    sequence.push_back(input_t{{"time", "20"}, {"x", "-15"}, {"y", "12"}});
    sequence.push_back(input_t{{"time", "42"}, {"x", "-13"}, {"y", "17"}});

    auto net1 = reelay::dense_timed<dense_time_t>::robustness<value_t>::monitor<
        input_t>::from_temporal_logic("{x>0} implies {y>0}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1->update(s);
      result1 = result1 | net1->output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 10), 11));
    expected1.add(std::make_pair(interval::left_open(10, 20), 15));
    expected1.add(std::make_pair(interval::left_open(20, 42), 17));

    CHECK(result1 == expected1);
  }
}

TEST_CASE("Untimed Temporal Operations") {

  SECTION("Past Always") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "10"}, {"x", "-11"}, {"y", "10"}});
    sequence.push_back(input_t{{"time", "20"}, {"x", "-15"}, {"y", "9"}});
    sequence.push_back(input_t{{"time", "42"}, {"x", "-13"}, {"y", "7"}});

    auto net1 = reelay::dense_timed<dense_time_t>::robustness<value_t>::monitor<
        input_t>::from_temporal_logic("historically{x>0}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1->update(s);
      result1 = result1 | net1->output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 10), -11));
    expected1.add(std::make_pair(interval::left_open(10, 20), -15));
    expected1.add(std::make_pair(interval::left_open(20, 42), -15));

    CHECK(result1 == expected1);
  }

  SECTION("Once") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "10"}, {"x", "11"}, {"y", "10"}});
    sequence.push_back(input_t{{"time", "20"}, {"x", "15"}, {"y", "9"}});
    sequence.push_back(input_t{{"time", "42"}, {"x", "13"}, {"y", "7"}});

    auto net1 = reelay::dense_timed<dense_time_t>::robustness<value_t>::monitor<
        input_t>::from_temporal_logic("once{x>0}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1->update(s);
      result1 = result1 | net1->output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 10), 11));
    expected1.add(std::make_pair(interval::left_open(10, 20), 15));
    expected1.add(std::make_pair(interval::left_open(20, 42), 15));

    CHECK(result1 == expected1);
  }

  SECTION("Since") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "1"}, {"x", "3"}, {"y", "0"}});
    sequence.push_back(input_t{{"time", "2"}, {"x", "4"}, {"y", "1"}}); //
    sequence.push_back(input_t{{"time", "3"}, {"x", "5"}, {"y", "0"}});
    sequence.push_back(input_t{{"time", "4"}, {"x", "1"}, {"y", "0"}});
    sequence.push_back(input_t{{"time", "5"}, {"x", "-3"}, {"y", "0"}});
    sequence.push_back(input_t{{"time", "6"}, {"x", "-1"}, {"y", "0"}});
    sequence.push_back(input_t{{"time", "7"}, {"x", "1"}, {"y", "0"}});
    sequence.push_back(input_t{{"time", "8"}, {"x", "4"}, {"y", "1"}});
    sequence.push_back(input_t{{"time", "9"}, {"x", "-1"}, {"y", "0"}});
    sequence.push_back(input_t{{"time", "10"}, {"x", "1"}, {"y", "0"}});
    sequence.push_back(input_t{{"time", "11"}, {"x", "6"}, {"y", "0"}});
    sequence.push_back(input_t{{"time", "12"}, {"x", "1"}, {"y", "0"}});
    sequence.push_back(input_t{{"time", "13"}, {"x", "1"}, {"y", "5"}});
    sequence.push_back(input_t{{"time", "14"}, {"x", "1"}, {"y", "0"}});

    auto net1 = reelay::dense_timed<dense_time_t>::robustness<value_t>::monitor<
        input_t>::from_temporal_logic("{x>0} since {y>0}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1->update(s);
      // std::cout << net1->output() << std::endl;
      result1 = result1 | net1->output();
    }

    auto expected1 = robustness_map();

    expected1.add(std::make_pair(interval::left_open(0, 1), 0));
    expected1.add(std::make_pair(interval::left_open(1, 2), 1));
    expected1.add(std::make_pair(interval::left_open(2, 3), 1));
    expected1.add(std::make_pair(interval::left_open(3, 4), 1));
    expected1.add(std::make_pair(interval::left_open(4, 5), 0));
    expected1.add(std::make_pair(interval::left_open(5, 6), 0));
    expected1.add(std::make_pair(interval::left_open(6, 7), 0));
    expected1.add(std::make_pair(interval::left_open(7, 8), 1));
    expected1.add(std::make_pair(interval::left_open(8, 9), 0));
    expected1.add(std::make_pair(interval::left_open(9, 10), 0));
    expected1.add(std::make_pair(interval::left_open(10, 11), 0));
    expected1.add(std::make_pair(interval::left_open(11, 12), 0));
    expected1.add(std::make_pair(interval::left_open(12, 13), 5));
    expected1.add(std::make_pair(interval::left_open(13, 14), 1));

    CHECK(result1 == expected1);
  }
}

TEST_CASE("Timed Temporal Operations") {

  SECTION("Timed Past Always") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "10"}, {"x", "11"}});
    sequence.push_back(input_t{{"time", "20"}, {"x", "9"}});
    sequence.push_back(input_t{{"time", "30"}, {"x", "13"}});
    sequence.push_back(input_t{{"time", "60"}, {"x", "17"}});
    sequence.push_back(input_t{{"time", "70"}, {"x", "15"}});
    sequence.push_back(input_t{{"time", "90"}, {"x", "4"}});

    auto net1 = reelay::dense_timed<dense_time_t>::robustness<value_t>::monitor<
        input_t>::from_temporal_logic("historically[12:24]{x>0}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1->update(s);
      result1 = result1 | net1->output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 12), top));
    expected1.add(std::make_pair(interval::left_open(12, 22), 11));
    expected1.add(std::make_pair(interval::left_open(20, 44), 9));
    expected1.add(std::make_pair(interval::left_open(44, 54), 13));
    expected1.add(std::make_pair(interval::left_open(54, 72), 17));
    expected1.add(std::make_pair(interval::left_open(72, 82), 15));
    expected1.add(std::make_pair(interval::left_open(82, 90), 4));

    CHECK(result1 == expected1);
  }

  SECTION("Timed Past Always Inf") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "10"}, {"x", "11"}});
    sequence.push_back(input_t{{"time", "20"}, {"x", "9"}});
    sequence.push_back(input_t{{"time", "30"}, {"x", "13"}});
    sequence.push_back(input_t{{"time", "60"}, {"x", "17"}});
    sequence.push_back(input_t{{"time", "70"}, {"x", "15"}});
    sequence.push_back(input_t{{"time", "90"}, {"x", "4"}});

    auto net1 = reelay::dense_timed<dense_time_t>::robustness<value_t>::monitor<
        input_t>::from_temporal_logic("historically[15:]{x>0}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1->update(s);
      result1 = result1 | net1->output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 15), top));
    expected1.add(std::make_pair(interval::left_open(15, 25), 11));
    expected1.add(std::make_pair(interval::left_open(25, 85), 9));
    expected1.add(std::make_pair(interval::left_open(85, 90), 4));

    CHECK(result1 == expected1);
  }

  SECTION("Timed Once") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "10"}, {"x", "11"}});
    sequence.push_back(input_t{{"time", "20"}, {"x", "9"}});
    sequence.push_back(input_t{{"time", "30"}, {"x", "13"}});
    sequence.push_back(input_t{{"time", "60"}, {"x", "17"}});
    sequence.push_back(input_t{{"time", "70"}, {"x", "15"}});
    sequence.push_back(input_t{{"time", "90"}, {"x", "4"}});

    auto net1 = reelay::dense_timed<dense_time_t>::robustness<value_t>::monitor<
        input_t>::from_temporal_logic("once[12:24]{x>0}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1->update(s);
      result1 = result1 | net1->output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 12), bot));
    expected1.add(std::make_pair(interval::left_open(12, 32), 11));
    expected1.add(std::make_pair(interval::left_open(32, 42), 13));
    expected1.add(std::make_pair(interval::left_open(42, 84), 17));
    expected1.add(std::make_pair(interval::left_open(84, 90), 15));

    CHECK(result1 == expected1);
  }

  SECTION("Timed Once Inf") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "10"}, {"x", "11"}});
    sequence.push_back(input_t{{"time", "20"}, {"x", "9"}});
    sequence.push_back(input_t{{"time", "30"}, {"x", "13"}});
    sequence.push_back(input_t{{"time", "60"}, {"x", "17"}});
    sequence.push_back(input_t{{"time", "70"}, {"x", "15"}});
    sequence.push_back(input_t{{"time", "90"}, {"x", "4"}});

    auto net1 = reelay::dense_timed<dense_time_t>::robustness<value_t>::monitor<
        input_t>::from_temporal_logic("once[15:]{x>0}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1->update(s);
      result1 = result1 | net1->output();
    }

    auto expected1 = robustness_map();
    expected1.add(std::make_pair(interval::left_open(0, 15), bot));
    expected1.add(std::make_pair(interval::left_open(15, 35), 11));
    expected1.add(std::make_pair(interval::left_open(35, 45), 13));
    expected1.add(std::make_pair(interval::left_open(45, 90), 17));

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "1"}, {"x", "3"}, {"y", "0"}});
    sequence.push_back(input_t{{"time", "2"}, {"x", "4"}, {"y", "1"}}); //
    sequence.push_back(input_t{{"time", "3"}, {"x", "5"}, {"y", "0"}});
    sequence.push_back(input_t{{"time", "4"}, {"x", "1"}, {"y", "0"}});
    sequence.push_back(input_t{{"time", "5"}, {"x", "-3"}, {"y", "0"}});
    sequence.push_back(input_t{{"time", "6"}, {"x", "-1"}, {"y", "0"}});
    sequence.push_back(input_t{{"time", "7"}, {"x", "1"}, {"y", "0"}});
    sequence.push_back(input_t{{"time", "8"}, {"x", "4"}, {"y", "1"}});
    sequence.push_back(input_t{{"time", "9"}, {"x", "-1"}, {"y", "0"}});
    sequence.push_back(input_t{{"time", "10"}, {"x", "1"}, {"y", "0"}});
    sequence.push_back(input_t{{"time", "11"}, {"x", "6"}, {"y", "0"}});
    sequence.push_back(input_t{{"time", "12"}, {"x", "1"}, {"y", "0"}});
    sequence.push_back(input_t{{"time", "13"}, {"x", "1"}, {"y", "5"}});
    sequence.push_back(input_t{{"time", "14"}, {"x", "1"}, {"y", "0"}});

    auto net1 = reelay::dense_timed<dense_time_t>::robustness<value_t>::monitor<
        input_t>::from_temporal_logic("{x>0} since[:100] {y>0}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1->update(s);
      // std::cout << net1->output() << std::endl;
      result1 = result1 | net1->output();
    }

    auto expected1 = robustness_map();

    expected1.add(std::make_pair(interval::left_open(0, 1), 0));
    expected1.add(std::make_pair(interval::left_open(1, 2), 1));
    expected1.add(std::make_pair(interval::left_open(2, 3), 1));
    expected1.add(std::make_pair(interval::left_open(3, 4), 1));
    expected1.add(std::make_pair(interval::left_open(4, 5), 0));
    expected1.add(std::make_pair(interval::left_open(5, 6), 0));
    expected1.add(std::make_pair(interval::left_open(6, 7), 0));
    expected1.add(std::make_pair(interval::left_open(7, 8), 1));
    expected1.add(std::make_pair(interval::left_open(8, 9), 0));
    expected1.add(std::make_pair(interval::left_open(9, 10), 0));
    expected1.add(std::make_pair(interval::left_open(10, 11), 0));
    expected1.add(std::make_pair(interval::left_open(11, 12), 0));
    expected1.add(std::make_pair(interval::left_open(12, 13), 5));
    expected1.add(std::make_pair(interval::left_open(13, 14), 1));

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since 2") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "10"}, {"x", "-3"}, {"y", "-6"}});
    sequence.push_back(input_t{{"time", "20"}, {"x", "-4"}, {"y", "1"}});
    sequence.push_back(input_t{{"time", "30"}, {"x", "-5"}, {"y", "-8"}});
    sequence.push_back(input_t{{"time", "40"}, {"x", "15"}, {"y", "3"}});
    sequence.push_back(input_t{{"time", "50"}, {"x", "-3"}, {"y", "22"}});

    auto net1 = reelay::dense_timed<dense_time_t>::robustness<value_t>::monitor<
        input_t>::from_temporal_logic("{x>0} since[5:15] {y>0}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1->update(s);
      result1 = result1 | net1->output();
    }

    auto expected1 = robustness_map();

    expected1.add(std::make_pair(interval::left_open(0, 5), bot));
    expected1.add(std::make_pair(interval::left_open(5, 15), -6));
    expected1.add(std::make_pair(interval::left_open(15, 20), 1));
    expected1.add(std::make_pair(interval::left_open(20, 35), -5));
    expected1.add(std::make_pair(interval::left_open(35, 45), 3));
    expected1.add(std::make_pair(interval::left_open(45, 50), 22));

    CHECK(result1 == expected1);
  }

  SECTION("Timed Since Inf") {

    std::vector<input_t> sequence = std::vector<input_t>();

    sequence.push_back(input_t{{"time", "1"}, {"x", "-3"}, {"y", "-6"}});
    sequence.push_back(input_t{{"time", "2"}, {"x", "-4"}, {"y", "1"}});
    sequence.push_back(input_t{{"time", "3"}, {"x", "-5"}, {"y", "-8"}});
    sequence.push_back(input_t{{"time", "4"}, {"x", "15"}, {"y", "3"}});
    sequence.push_back(input_t{{"time", "5"}, {"x", "-3"}, {"y", "22"}});

    auto net1 = reelay::dense_timed<dense_time_t>::robustness<value_t>::monitor<
        input_t>::from_temporal_logic("{x>0} since[2:] {y>0}");

    auto result1 = robustness_map();

    for (const auto &s : sequence) {
      net1->update(s);
      result1 = result1 | net1->output();
    }

    auto expected1 = robustness_map();

    expected1.add(std::make_pair(interval::left_open(0, 1), bot));
    expected1.add(std::make_pair(interval::left_open(1, 2), bot));
    expected1.add(std::make_pair(interval::left_open(2, 3), -6));
    expected1.add(std::make_pair(interval::left_open(3, 4), -5));
    expected1.add(std::make_pair(interval::left_open(4, 5), -5));

    CHECK(result1 == expected1);
  }
}
/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "functional"
#include "memory"
#include "string"

#include "reelay/networks/basic_structure.hpp"
#include "reelay/networks/dense_timed_network.hpp"

#include "reelay/settings/dense_timed_data/atomic_listing_0.hpp"
#include "reelay/settings/dense_timed_data/atomic_record_0.hpp"
#include "reelay/settings/dense_timed_data/atomic_listing_1.hpp"
#include "reelay/settings/dense_timed_data/atomic_record_1.hpp"

#include "reelay/settings/dense_timed_data/exists.hpp"
#include "reelay/settings/dense_timed_data/forall.hpp"

#include "reelay/settings/dense_timed_data/conjunction.hpp"
#include "reelay/settings/dense_timed_data/disjunction.hpp"
#include "reelay/settings/dense_timed_data/implication.hpp"
#include "reelay/settings/dense_timed_data/negation.hpp"

#include "reelay/settings/dense_timed_data/past_always.hpp"
#include "reelay/settings/dense_timed_data/past_sometime.hpp"
#include "reelay/settings/dense_timed_data/since.hpp"

#include "reelay/settings/dense_timed_data/past_always_bounded.hpp"
#include "reelay/settings/dense_timed_data/past_sometime_bounded.hpp"
#include "reelay/settings/dense_timed_data/since_bounded.hpp"

#include "reelay/settings/dense_timed_data/past_always_bounded_half.hpp"
#include "reelay/settings/dense_timed_data/past_sometime_bounded_half.hpp"
#include "reelay/settings/dense_timed_data/since_bounded_half.hpp"

namespace reelay {
namespace dense_timed_data_setting {

template <typename X, typename T, int option> struct factory {
  using time_t = T;
  using input_t = X;
  using value_t = data_set_t;
  using output_t = reelay::data_interval_map<time_t>;

  using node_t = dense_timed_node<output_t, time_t>;
  using state_t = dense_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  using interval = reelay::interval<time_t>;
  using interval_map = reelay::data_interval_map<time_t>;

  using network_t =
      reelay::dense_timed_network<input_t, output_t, time_t, value_t>;
  using network_ptr_t = std::shared_ptr<network_t>;

  using function_t =
      std::function<output_t(const input_t &, const input_t &, time_t, time_t)>;

  static node_ptr_t make_node(const std::string &name, const kwargs &kw) {

    node_ptr_t result;

    if (name == "disjunction") {
      result = std::make_shared<disjunction<input_t, time_t>>(kw);
    } else if (name == "conjunction") {
      result = std::make_shared<conjunction<input_t, time_t>>(kw);
    } else if (name == "negation") {
      result = std::make_shared<negation<input_t, time_t>>(kw);
    } else if (name == "implication") {
      result = std::make_shared<implication<input_t, time_t>>(kw);
    } else if (name == "exists") {
      result = std::make_shared<exists<input_t, time_t>>(kw);
    } else if (name == "forall") {
      result = std::make_shared<forall<input_t, time_t>>(kw);
    } else {
      throw std::invalid_argument(
          "Unsupported operator for the untimed setting");
    }

    return result;
  }

  static state_ptr_t make_state(const std::string &name, const kwargs &kw) {

    state_ptr_t res;

    if (name == "proposition") {
      // res = std::make_shared<proposition<input_t, time_t>>(kw);
    } else if (name == "listing") {
      res = std::make_shared<listing<input_t, time_t, option>>(kw);
    } else if (name == "record") {
      res = std::make_shared<record<input_t, time_t, option>>(kw);
    } else if (name == "past_sometime") {
      res = std::make_shared<past_sometime<input_t, time_t>>(kw);
    } else if (name == "past_always") {
      res = std::make_shared<past_always<input_t, time_t>>(kw);
    } else if (name == "since") {
      res = std::make_shared<since<input_t, time_t>>(kw);
    } else if (name == "past_sometime_bounded") {
      res = std::make_shared<past_sometime_bounded<input_t, time_t>>(kw);
    } else if (name == "past_always_bounded") {
      res = std::make_shared<past_always_bounded<input_t, time_t>>(kw);
    } else if (name == "since_bounded") {
      res = std::make_shared<since_bounded<input_t, time_t>>(kw);
    } else if (name == "past_sometime_bounded_half") {
      res = std::make_shared<past_sometime_bounded_half<input_t, time_t>>(kw);
    } else if (name == "past_always_bounded_half") {
      res = std::make_shared<past_always_bounded_half<input_t, time_t>>(kw);
    } else if (name == "since_bounded_half") {
      res = std::make_shared<since_bounded_half<input_t, time_t>>(kw);
      // } else if (name == "predicate") {
      //   res = std::make_shared<predicate<input_t, time_t>>(kw);
    } else {
      throw std::invalid_argument(
          "Unsupported operator for the untimed setting");
    }

    return res;
  }
};

} // namespace dense_timed_data_setting
} // namespace reelay
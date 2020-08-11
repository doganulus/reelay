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
//
#include "reelay/settings/dense_timed_data/atomic_any.hpp"
#include "reelay/settings/dense_timed_data/atomic_false.hpp"
#include "reelay/settings/dense_timed_data/atomic_ge_0.hpp"
#include "reelay/settings/dense_timed_data/atomic_gt_0.hpp"
#include "reelay/settings/dense_timed_data/atomic_le_0.hpp"
#include "reelay/settings/dense_timed_data/atomic_lt_0.hpp"
// #include "reelay/settings/dense_timed_data/atomic_ne.hpp"
// #include "reelay/settings/dense_timed_data/atomic_eq.hpp"
#include "reelay/settings/dense_timed_data/atomic_number.hpp"
#include "reelay/settings/dense_timed_data/atomic_prop.hpp"
#include "reelay/settings/dense_timed_data/atomic_ref.hpp"
#include "reelay/settings/dense_timed_data/atomic_string.hpp"
#include "reelay/settings/dense_timed_data/atomic_true.hpp"
#include "reelay/settings/dense_timed_data/atomic_map.hpp"

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

template <typename X, typename T, int order=0> struct factory {
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
          "Unsupported operator for the dense timed data setting");
    }

    return result;
  }

  static state_ptr_t make_state(const std::string &name, kwargs &kw) {

    state_ptr_t res;

    if (name == "atomic_map") {
      res = std::make_shared<atomic_map<input_t, time_t>>(kw);
    } else if (name == "mapping_prop") {
      res = std::make_shared<atomic_prop<input_t, time_t, std::string>>(kw);
    } else if (name == "mapping_false") {
      res = std::make_shared<atomic_false<input_t, time_t, std::string>>(kw);
    } else if (name == "mapping_true") {
      res = std::make_shared<atomic_true<input_t, time_t, std::string>>(kw);
    } else if (name == "mapping_string") {
      res = std::make_shared<atomic_string<input_t, time_t, std::string>>(kw);
    } else if (name == "mapping_number") {
      res = std::make_shared<atomic_number<input_t, time_t, std::string>>(kw);
    } else if (name == "mapping_eq") {
      res = std::make_shared<atomic_number<input_t, time_t, std::string>>(kw);
    } else if (name == "mapping_ge" and order == 0) {
      res = std::make_shared<atomic_ge_0<input_t, time_t, std::string>>(kw);
    } else if (name == "mapping_gt" and order == 0) {
      res = std::make_shared<atomic_gt_0<input_t, time_t, std::string>>(kw);
    } else if (name == "mapping_le" and order == 0) {
      res = std::make_shared<atomic_le_0<input_t, time_t, std::string>>(kw);
    } else if (name == "mapping_lt" and order == 0) {
      res = std::make_shared<atomic_lt_0<input_t, time_t, std::string>>(kw);
    } else if (name == "mapping_any") {
      res = std::make_shared<atomic_any<input_t, time_t, std::string>>(kw);
    } else if (name == "mapping_ref") {
      res = std::make_shared<atomic_ref<input_t, time_t, std::string>>(kw);
    } else if (name == "listing_false") {
      res = std::make_shared<atomic_false<input_t, time_t, int>>(kw);
    } else if (name == "listing_true") {
      res = std::make_shared<atomic_true<input_t, time_t, int>>(kw);
    } else if (name == "listing_string") {
      res = std::make_shared<atomic_string<input_t, time_t, int>>(kw);
    } else if (name == "listing_number") {
      res = std::make_shared<atomic_number<input_t, time_t, int>>(kw);
    } else if (name == "listing_eq") {
      res = std::make_shared<atomic_number<input_t, time_t, int>>(kw);
    } else if (name == "listing_ge" and order == 0) {
      kw["key"] = reelay::any_cast<int>(kw.at("key")) + 1;
      res = std::make_shared<atomic_ge_0<input_t, time_t, int>>(kw);
    } else if (name == "listing_gt" and order == 0) {
      kw["key"] = reelay::any_cast<int>(kw.at("key")) + 1;
      res = std::make_shared<atomic_gt_0<input_t, time_t, int>>(kw);
    } else if (name == "listing_le" and order == 0) {
      kw["key"] = reelay::any_cast<int>(kw.at("key")) + 1;
      res = std::make_shared<atomic_le_0<input_t, time_t, int>>(kw);
    } else if (name == "listing_lt" and order == 0) {
      kw["key"] = reelay::any_cast<int>(kw.at("key")) + 1;
      res = std::make_shared<atomic_lt_0<input_t, time_t, int>>(kw);
    } else if (name == "listing_any") {
      kw["key"] = reelay::any_cast<int>(kw.at("key")) + 1;
      res = std::make_shared<atomic_any<input_t, time_t, int>>(kw);
    } else if (name == "listing_ref") {
      kw["key"] = reelay::any_cast<int>(kw.at("key")) + 1;
      res = std::make_shared<atomic_ref<input_t, time_t, int>>(kw);
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
    } else {
      throw std::invalid_argument(
          "Unsupported operator for the dense timed data setting");
    }

    return res;
  }
};

} // namespace dense_timed_data_setting
} // namespace reelay
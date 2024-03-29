/*
 * Copyright (c) 2019-2023 Dogan Ulus
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
#include "reelay/settings/dense_timed_robustness_0/atomic_any.hpp"
#include "reelay/settings/dense_timed_robustness_0/atomic_false.hpp"
#include "reelay/settings/dense_timed_robustness_0/atomic_ge_0.hpp"
#include "reelay/settings/dense_timed_robustness_0/atomic_gt_0.hpp"
#include "reelay/settings/dense_timed_robustness_0/atomic_le_0.hpp"
#include "reelay/settings/dense_timed_robustness_0/atomic_lt_0.hpp"
// #include "reelay/settings/dense_timed_robustness_0/atomic_ne.hpp"
// #include "reelay/settings/dense_timed_robustness_0/atomic_eq.hpp"
#include "reelay/settings/dense_timed_robustness_0/atomic_number.hpp"
#include "reelay/settings/dense_timed_robustness_0/atomic_prop.hpp"
#include "reelay/settings/dense_timed_robustness_0/atomic_string.hpp"
#include "reelay/settings/dense_timed_robustness_0/atomic_true.hpp"
#include "reelay/settings/dense_timed_robustness_0/atomic_map.hpp"

#include "reelay/settings/dense_timed_robustness_0/conjunction.hpp"
#include "reelay/settings/dense_timed_robustness_0/disjunction.hpp"
#include "reelay/settings/dense_timed_robustness_0/implication.hpp"
#include "reelay/settings/dense_timed_robustness_0/negation.hpp"

#include "reelay/settings/dense_timed_robustness_0/past_always.hpp"
#include "reelay/settings/dense_timed_robustness_0/past_sometime.hpp"
#include "reelay/settings/dense_timed_robustness_0/since.hpp"

#include "reelay/settings/dense_timed_robustness_0/past_always_bounded.hpp"
#include "reelay/settings/dense_timed_robustness_0/past_sometime_bounded.hpp"
#include "reelay/settings/dense_timed_robustness_0/since_bounded.hpp"

#include "reelay/settings/dense_timed_robustness_0/past_always_bounded_half.hpp"
#include "reelay/settings/dense_timed_robustness_0/past_sometime_bounded_half.hpp"
#include "reelay/settings/dense_timed_robustness_0/since_bounded_half.hpp"

namespace reelay {
namespace dense_timed_robustness_0_setting {

template <typename X, typename V, typename T> struct factory {
  using time_t = T;
  using input_t = X;
  using value_t = V;
  using output_t = reelay::robustness_interval_map<time_t, value_t>;

  using interval = reelay::interval<time_t>;
  using interval_map = reelay::robustness_interval_map<time_t, value_t>;

  using node_t = dense_timed_node<output_t, time_t>;
  using state_t = dense_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  using function_t =
      std::function<output_t(const input_t &, const input_t &, time_t, time_t)>;

  static node_ptr_t make_node(const std::string &name, const kwargs &kw) {

    node_ptr_t result;

    if (name == "disjunction") {
      result = std::make_shared<disjunction<input_t, value_t, time_t>>(kw);
    } else if (name == "conjunction") {
      result = std::make_shared<conjunction<input_t, value_t, time_t>>(kw);
    } else if (name == "negation") {
      result = std::make_shared<negation<input_t, value_t, time_t>>(kw);
    } else if (name == "implication") {
      result = std::make_shared<implication<input_t, value_t, time_t>>(kw);
    } else {
      throw std::invalid_argument(
          "Unsupported operator for the dense timed robustness setting");
    }

    return result;
  }

  static state_ptr_t make_state(const std::string &name, const kwargs &kw) {

    state_ptr_t result;

    if (name == "atomic_map") {
      result = std::make_shared<atomic_map<input_t, value_t, time_t>>(kw);
    } else if (name == "mapping_prop") {
      result = std::make_shared<atomic_prop<input_t, value_t, time_t>>(kw);
    } else if (name == "mapping_false") {
      result = std::make_shared<atomic_false<input_t, value_t, time_t>>(kw);
    } else if (name == "mapping_true") {
      result = std::make_shared<atomic_true<input_t, value_t, time_t>>(kw);
    } else if (name == "mapping_string") {
      result = std::make_shared<atomic_string<input_t, value_t, time_t>>(kw);
    } else if (name == "mapping_number") {
      result = std::make_shared<atomic_number<input_t, value_t, time_t>>(kw);
    } else if (name == "mapping_ge") {
      result = std::make_shared<atomic_ge_0<input_t, value_t, time_t>>(kw);
    } else if (name == "mapping_gt") {
      result = std::make_shared<atomic_gt_0<input_t, value_t, time_t>>(kw);
    } else if (name == "mapping_le") {
      result = std::make_shared<atomic_le_0<input_t, value_t, time_t>>(kw);
    } else if (name == "mapping_lt") {
      result = std::make_shared<atomic_lt_0<input_t, value_t, time_t>>(kw);
    } else if (name == "mapping_any") {
      result = std::make_shared<atomic_any<input_t, value_t, time_t>>(kw);
    } else if (name == "past_sometime") {
      result = std::make_shared<past_sometime<input_t, value_t, time_t>>(kw);
    } else if (name == "past_always") {
      result = std::make_shared<past_always<input_t, value_t, time_t>>(kw);
    } else if (name == "since") {
      result = std::make_shared<since<input_t, value_t, time_t>>(kw);
    } else if (name == "past_sometime_bounded") {
      result =
          std::make_shared<past_sometime_bounded<input_t, value_t, time_t>>(kw);
    } else if (name == "past_always_bounded") {
      result =
          std::make_shared<past_always_bounded<input_t, value_t, time_t>>(kw);
    } else if (name == "since_bounded") {
      result = std::make_shared<since_bounded<input_t, value_t, time_t>>(kw);
    } else if (name == "past_sometime_bounded_half") {
      result = std::make_shared<
          past_sometime_bounded_half<input_t, value_t, time_t>>(kw);
    } else if (name == "past_always_bounded_half") {
      result =
          std::make_shared<past_always_bounded_half<input_t, value_t, time_t>>(
              kw);
    } else if (name == "since_bounded_half") {
      result =
          std::make_shared<since_bounded_half<input_t, value_t, time_t>>(kw);
    } else {
      throw std::invalid_argument(
          "Unsupported operator for the untimed setting");
    }

    return result;
  }
};

} // namespace dense_timed_robustness_0_setting
} // namespace reelay
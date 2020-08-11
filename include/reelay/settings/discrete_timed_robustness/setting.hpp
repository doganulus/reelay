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

#include "reelay/intervals.hpp"
#include "reelay/networks/basic_structure.hpp"
//
#include "reelay/settings/discrete_timed_robustness/atomic_any.hpp"
#include "reelay/settings/discrete_timed_robustness/atomic_eq.hpp"
#include "reelay/settings/discrete_timed_robustness/atomic_false.hpp"
#include "reelay/settings/discrete_timed_robustness/atomic_ge.hpp"
#include "reelay/settings/discrete_timed_robustness/atomic_gt.hpp"
#include "reelay/settings/discrete_timed_robustness/atomic_le.hpp"
#include "reelay/settings/discrete_timed_robustness/atomic_lt.hpp"
#include "reelay/settings/discrete_timed_robustness/atomic_ne.hpp"
#include "reelay/settings/discrete_timed_robustness/atomic_number.hpp"
#include "reelay/settings/discrete_timed_robustness/atomic_prop.hpp"
#include "reelay/settings/discrete_timed_robustness/atomic_string.hpp"
#include "reelay/settings/discrete_timed_robustness/atomic_true.hpp"
#include "reelay/settings/discrete_timed_robustness/atomic_map.hpp"

#include "reelay/settings/discrete_timed_robustness/conjunction.hpp"
#include "reelay/settings/discrete_timed_robustness/disjunction.hpp"
#include "reelay/settings/discrete_timed_robustness/implication.hpp"
#include "reelay/settings/discrete_timed_robustness/negation.hpp"

#include "reelay/settings/discrete_timed_robustness/past_always.hpp"
#include "reelay/settings/discrete_timed_robustness/past_sometime.hpp"
#include "reelay/settings/discrete_timed_robustness/previous.hpp"
#include "reelay/settings/discrete_timed_robustness/since.hpp"

#include "reelay/settings/discrete_timed_robustness/past_always_bounded.hpp"
#include "reelay/settings/discrete_timed_robustness/past_sometime_bounded.hpp"
#include "reelay/settings/discrete_timed_robustness/since_bounded.hpp"

#include "reelay/settings/discrete_timed_robustness/past_always_bounded_half.hpp"
#include "reelay/settings/discrete_timed_robustness/past_sometime_bounded_half.hpp"
#include "reelay/settings/discrete_timed_robustness/since_bounded_half.hpp"

namespace reelay {
namespace discrete_timed_robustness_setting {

template <typename X, typename Y, typename T> 
struct factory {
  using input_t = X;
  using time_t = T;
  using value_t = Y;
  using output_t = Y;

  using function_t = std::function<output_t(const input_t&)>;

  using interval = reelay::interval<time_t>;
  using interval_set = reelay::interval_set<time_t>;
  using interval_map = reelay::robustness_interval_map<time_t, output_t>;

  using node_t = reelay::discrete_timed_node<output_t, time_t>;
  using state_t = reelay::discrete_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  constexpr static output_t top = std::numeric_limits<output_t>::max();
  constexpr static output_t bottom = -std::numeric_limits<output_t>::max();

  static node_ptr_t make_node(const std::string &name, const kwargs &kw) {

    node_ptr_t result;

    if (name == "disjunction") {
      result = std::make_shared<disjunction<input_t, output_t, time_t>>(kw);
    } else if (name == "conjunction") {
      result = std::make_shared<conjunction<input_t, output_t, time_t>>(kw);
    } else if (name == "negation") {
      result = std::make_shared<negation<input_t, output_t, time_t>>(kw);
    } else if (name == "implication") {
      result = std::make_shared<implication<input_t, output_t, time_t>>(kw);
    } else {
      throw std::invalid_argument(
          "Unsupported operator for the discrete timed robustness setting");
    }
    return result;
  }

  static state_ptr_t make_state(const std::string &name, const kwargs &kw) {

    state_ptr_t result;

    if (name == "atomic_map") {
      result = std::make_shared<atomic_map<input_t, output_t, time_t>>(kw);
    } else if(name == "mapping_prop") {
      result = std::make_shared<atomic_prop<input_t, output_t, time_t>>(kw);
    } else if (name == "mapping_false") {
      result = std::make_shared<atomic_false<input_t, output_t, time_t>>(kw);
    } else if (name == "mapping_true") {
      result = std::make_shared<atomic_true<input_t, output_t, time_t>>(kw);
    } else if (name == "mapping_string") {
      result = std::make_shared<atomic_string<input_t, output_t, time_t>>(kw);
    } else if (name == "mapping_number") {
      result = std::make_shared<atomic_number<input_t, output_t, time_t>>(kw);
    } else if (name == "mapping_eq") {
      result = std::make_shared<atomic_eq<input_t, output_t, time_t>>(kw);
    } else if (name == "mapping_ne") {
      result = std::make_shared<atomic_ne<input_t, output_t, time_t>>(kw);
    } else if (name == "mapping_ge") {
      result = std::make_shared<atomic_ge<input_t, output_t, time_t>>(kw);
    } else if (name == "mapping_gt") {
      result = std::make_shared<atomic_gt<input_t, output_t, time_t>>(kw);
    } else if (name == "mapping_le") {
      result = std::make_shared<atomic_le<input_t, output_t, time_t>>(kw);
    } else if (name == "mapping_lt") {
      result = std::make_shared<atomic_lt<input_t, output_t, time_t>>(kw);
    } else if (name == "mapping_any") {
      result = std::make_shared<atomic_any<input_t, output_t, time_t>>(kw);
    } else if (name == "previous") {
      result = std::make_shared<previous<input_t, output_t, time_t>>(kw);
    } else if (name == "past_sometime") {
      result = std::make_shared<past_sometime<input_t, output_t, time_t>>(kw);
    } else if (name == "past_always") {
      result = std::make_shared<past_always<input_t, output_t, time_t>>(kw);
    } else if (name == "since") {
      result = std::make_shared<since<input_t, output_t, time_t>>(kw);
    } else if (name == "past_sometime_bounded") {
      result =
          std::make_shared<past_sometime_bounded<input_t, output_t, time_t>>(
              kw);
    } else if (name == "past_always_bounded") {
      result =
          std::make_shared<past_always_bounded<input_t, output_t, time_t>>(kw);
    } else if (name == "since_bounded") {
      result = std::make_shared<since_bounded<input_t, output_t, time_t>>(kw);
    } else if (name == "past_sometime_bounded_half") {
      result = std::make_shared<
          past_sometime_bounded_half<input_t, output_t, time_t>>(kw);
    } else if (name == "past_always_bounded_half") {
      result =
          std::make_shared<past_always_bounded_half<input_t, output_t, time_t>>(
              kw);
    } else if (name == "since_bounded_half") {
      result =
          std::make_shared<since_bounded_half<input_t, output_t, time_t>>(kw);
    } else {
      throw std::invalid_argument(
          "Unsupported operator for the discrete timed robustness setting");
    }
    return result;
  }

};

}  // namespace discrete_timed_robustness_setting
}  // namespace reelay
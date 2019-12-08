/*
 * Copyright (c) 2019 Dogan Ulus
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
#include "reelay/networks/discrete_timed_network.hpp"

#include "reelay/past_temporal_logic/discrete_timed/atoms.hpp"

#include "reelay/past_temporal_logic/discrete_timed/conjunction.hpp"
#include "reelay/past_temporal_logic/discrete_timed/disjunction.hpp"
#include "reelay/past_temporal_logic/discrete_timed/implication.hpp"
#include "reelay/past_temporal_logic/discrete_timed/negation.hpp"

#include "reelay/past_temporal_logic/discrete_timed/past_always.hpp"
#include "reelay/past_temporal_logic/discrete_timed/past_sometime.hpp"
#include "reelay/past_temporal_logic/discrete_timed/previous.hpp"
#include "reelay/past_temporal_logic/discrete_timed/since.hpp"

#include "reelay/past_temporal_logic/discrete_timed/past_always_bounded.hpp"
#include "reelay/past_temporal_logic/discrete_timed/past_sometime_bounded.hpp"
#include "reelay/past_temporal_logic/discrete_timed/since_bounded.hpp"

#include "reelay/past_temporal_logic/discrete_timed/past_always_bounded_half.hpp"
#include "reelay/past_temporal_logic/discrete_timed/past_sometime_bounded_half.hpp"
#include "reelay/past_temporal_logic/discrete_timed/since_bounded_half.hpp"

namespace reelay {
namespace discrete_timed_setting {

template <class X, class T>
struct factory {
  using input_t = X;
  using time_t = T;

  using value_t = bool;
  using output_t = bool;

  using function_t = std::function<bool(const input_t&)>;

  using interval = reelay::interval<time_t>;
  using interval_set = reelay::interval_set<time_t>;

  using node_t = reelay::discrete_timed_node<output_t, time_t>;
  using state_t = reelay::discrete_timed_state<input_t, output_t, time_t>;
  using network_t = reelay::discrete_timed_network<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;
  using network_ptr_t = std::shared_ptr<network_t>;

  const static output_t top = true;
  const static output_t bottom = false;

  static state_ptr_t make_proposition(const std::string& name) {
    return std::make_shared<
        discrete_timed_setting::proposition<input_t, time_t>>(name);
  }

  static state_ptr_t make_basic_predicate(const std::string& name,
                                          const std::string& op,
                                          float c) {
    state_ptr_t result;

    if (op == "lt" or op == "<") {
      result = std::make_shared<
          discrete_timed_setting::basic_predicate_lt<input_t, time_t>>(name, c);
    } else if (op == "le" or op == "leq" or op == "<=") {
      result = std::make_shared<
          discrete_timed_setting::basic_predicate_le<input_t, time_t>>(name, c);
    } else if (op == "gt" or op == ">") {
      result = std::make_shared<
          discrete_timed_setting::basic_predicate_gt<input_t, time_t>>(name, c);
    } else if (op == "ge" or op == "geq" or op == ">=") {
      result = std::make_shared<
          discrete_timed_setting::basic_predicate_ge<input_t, time_t>>(name, c);
    } else {
      throw std::invalid_argument(
          "Unsupported predicate operator for the discrete timed setting");
    }

    return result;
  }

  static state_ptr_t make_predicate(const function_t& fn) {
    return std::make_shared<discrete_timed_setting::predicate<input_t, time_t>>(
        fn);
  }

  static node_ptr_t make_node(const std::string& name,
                              const std::vector<node_ptr_t>& args) {
    node_ptr_t result;

    if (name == "disjunction") {
      result = std::make_shared<
          discrete_timed_setting::disjunction<input_t, time_t>>(args);
    } else if (name == "conjunction") {
      result = std::make_shared<
          discrete_timed_setting::conjunction<input_t, time_t>>(args);
    } else if (name == "negation") {
      result =
          std::make_shared<discrete_timed_setting::negation<input_t, time_t>>(
              args);
    } else if (name == "implication") {
      result = std::make_shared<
          discrete_timed_setting::implication<input_t, time_t>>(args);
    } else if (name == "past_sometime") {
      result = std::make_shared<
          discrete_timed_setting::past_sometime<input_t, time_t>>(args);
    } else if (name == "past_always") {
      result = std::make_shared<
          discrete_timed_setting::past_always<input_t, time_t>>(args);
    } else if (name == "since") {
      result = std::make_shared<discrete_timed_setting::since<input_t, time_t>>(
          args);
    } else {
      throw std::invalid_argument(
          "Unsupported operator for the discrete timed setting");
    }

    return result;
  }

  static state_ptr_t make_state(
      const std::string& name,
      const std::vector<node_ptr_t>& args,
      const std::pair<time_t, time_t>& bounds = std::make_pair(0, 0)) {
    state_ptr_t result;

    if (name == "past_sometime") {
      result = std::make_shared<
          discrete_timed_setting::past_sometime<input_t, time_t>>(args);
    } else if (name == "past_always") {
      result = std::make_shared<
          discrete_timed_setting::past_always<input_t, time_t>>(args);
    } else if (name == "previous") {
      result = std::make_shared<
          discrete_timed_setting::previous<input_t, time_t>>(args);
    } else if (name == "since") {
      result = std::make_shared<discrete_timed_setting::since<input_t, time_t>>(
          args);
    } else if (name == "past_sometime_bounded") {
      time_t l = std::get<0>(bounds);
      time_t u = std::get<1>(bounds);

      if (u > 0) {
        result = std::make_shared<
            discrete_timed_setting::past_sometime_bounded<input_t, time_t>>(
            args, l, u);
      } else {
        result =
            std::make_shared<discrete_timed_setting::past_sometime_bounded_half<
                input_t, time_t>>(args, l);
      }

    } else if (name == "past_always_bounded") {
      time_t l = std::get<0>(bounds);
      time_t u = std::get<1>(bounds);

      if (u > 0) {
        result = std::make_shared<
            discrete_timed_setting::past_always_bounded<input_t, time_t>>(args,
                                                                          l, u);
      } else {
        result = std::make_shared<
            discrete_timed_setting::past_always_bounded_half<input_t, time_t>>(
            args, l);
      }

    } else if (name == "since_bounded") {
      time_t l = std::get<0>(bounds);
      time_t u = std::get<1>(bounds);

      if (u > 0) {
        result = std::make_shared<
            discrete_timed_setting::since_bounded<input_t, time_t>>(args, l, u);
      } else {
        result = std::make_shared<
            discrete_timed_setting::since_bounded_half<input_t, time_t>>(args,
                                                                         l);
      }

    } else {
      throw std::invalid_argument(
          "Unsupported operator for the discrete timed setting");
    }

    return result;
  }
};

}  // namespace discrete_timed_setting
}  // namespace reelay
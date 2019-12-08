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
#include "limits"

#include "reelay/networks/basic_structure.hpp"
#include "reelay/networks/untimed_network.hpp"

#include "reelay/past_temporal_logic/untimed_robustness/atoms.hpp"

#include "reelay/past_temporal_logic/untimed_robustness/conjunction.hpp"
#include "reelay/past_temporal_logic/untimed_robustness/disjunction.hpp"
#include "reelay/past_temporal_logic/untimed_robustness/implication.hpp"
#include "reelay/past_temporal_logic/untimed_robustness/negation.hpp"

#include "reelay/past_temporal_logic/untimed_robustness/past_always.hpp"
#include "reelay/past_temporal_logic/untimed_robustness/past_sometime.hpp"
#include "reelay/past_temporal_logic/untimed_robustness/previous.hpp"
#include "reelay/past_temporal_logic/untimed_robustness/since.hpp"

namespace reelay {
namespace untimed_robustness_setting {

template <typename X, typename Y> 
struct factory {
  using input_t = X;
  using value_t = Y;
  using output_t = Y;

  using function_t = std::function<output_t(const input_t&)>;

  using node_t = reelay::untimed_node<output_t>;
  using state_t = reelay::untimed_state<input_t, output_t>;
  using network_t = reelay::untimed_network<input_t, output_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;
  using network_ptr_t = std::shared_ptr<network_t>;

  constexpr static output_t top = std::numeric_limits<output_t>::max();
  constexpr static output_t bottom = -std::numeric_limits<output_t>::max();

  static state_ptr_t make_proposition(const std::string& name) {
    return std::make_shared<proposition<input_t, output_t>>(name);
  }

  static state_ptr_t make_basic_predicate(const std::string& name,
                                          const std::string& op,
                                          float c) {
    state_ptr_t result;

    if (op == "lt" or op == "<") {
      result = std::make_shared<
          basic_predicate_lt<input_t, output_t>>(name,
                                                                         c);
    } else if (op == "le" or op == "leq" or op == "<=") {
      result = std::make_shared<
          basic_predicate_le<input_t, output_t>>(name,
                                                                         c);
    } else if (op == "gt" or op == ">") {
      result = std::make_shared<
          basic_predicate_gt<input_t, output_t>>(name,
                                                                         c);
    } else if (op == "ge" or op == "geq" or op == ">=") {
      result = std::make_shared<
          basic_predicate_ge<input_t, output_t>>(name,
                                                                         c);
    } else {
      throw std::invalid_argument(
          "Unsupported predicate operator for the discrete timed setting");
    }

    return result;
  }

  static state_ptr_t make_predicate(const function_t& fn) {
    return std::make_shared<predicate<input_t, output_t>>(fn);
  }

  static node_ptr_t make_node(const std::string& name,
                              const std::vector<node_ptr_t>& args) {
    node_ptr_t result;

    if (name == "disjunction") {
      result =
          std::make_shared<disjunction<input_t, output_t>>(
              args);
    } else if (name == "conjunction") {
      result = std::make_shared<
          conjunction<input_t, output_t>>(args);
    } else if (name == "negation") {
      result =
          std::make_shared<negation<input_t, output_t>>(
              args);
    } else if (name == "implication") {
      result = std::make_shared<
          implication<input_t, output_t>>(args);
    } else if (name == "past_sometime") {
      result = std::make_shared<
          past_sometime<input_t, output_t>>(args);
    } else if (name == "past_always") {
      result = std::make_shared<
          past_always<input_t, output_t>>(args);
    } else if (name == "since") {
      result =
          std::make_shared<since<input_t, output_t>>(
              args);
    } else {
      throw std::invalid_argument(
          "Unsupported operator for the untimed setting");
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
          past_sometime<input_t, output_t>>(args);
    } else if (name == "past_always") {
      result = std::make_shared<
          past_always<input_t, output_t>>(args);
    } else if (name == "previous") {
      result = std::make_shared<previous<input_t, output_t>>(args);
    } else if (name == "since") {
      result =
          std::make_shared<since<input_t, output_t>>(
              args);
    } else {
      throw std::invalid_argument(
          "Unsupported operator for the untimed setting");
    }

    return result;
  }
};

} // namespace untimed_robustness_setting
}  // namespace reelay
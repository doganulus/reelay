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


#include "reelay/networks/basic_structure.hpp"
#include "reelay/networks/untimed_network.hpp"

#include "reelay/past_temporal_logic/untimed/atoms.hpp"

#include "reelay/past_temporal_logic/untimed/conjunction.hpp"
#include "reelay/past_temporal_logic/untimed/disjunction.hpp"
#include "reelay/past_temporal_logic/untimed/implication.hpp"
#include "reelay/past_temporal_logic/untimed/negation.hpp"


#include "reelay/past_temporal_logic/untimed/past_always.hpp"
#include "reelay/past_temporal_logic/untimed/past_sometime.hpp"
#include "reelay/past_temporal_logic/untimed/previous.hpp"
#include "reelay/past_temporal_logic/untimed/since.hpp"

namespace reelay {
namespace untimed_setting {

template <typename X>
struct factory {
  using input_t = X;
  using value_t = bool;
  using output_t = bool;
  
  using function_t = std::function<bool(const input_t&)>;

  using node_t = reelay::untimed_node<output_t>;
  using state_t = reelay::untimed_state<input_t, output_t>;
  using network_t = reelay::untimed_network<input_t, output_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;
  using network_ptr_t = std::shared_ptr<network_t>;

  const static output_t top = true;
  const static output_t bottom = false;

  static state_ptr_t make_proposition(const std::string& name) {
    return std::make_shared<untimed_setting::proposition<input_t>>(name);
  }

  static state_ptr_t make_basic_predicate(const std::string& name,
                                          const std::string& op,
                                          float c) {
    state_ptr_t result;

    if (op == "lt" or op == "<") {
      result = std::make_shared<untimed_setting::basic_predicate_lt<input_t>>(
          name, c);
    } else if (op == "le" or op == "leq" or op == "<=") {
      result = std::make_shared<untimed_setting::basic_predicate_le<input_t>>(
          name, c);
    } else if (op == "gt" or op == ">") {
      result = std::make_shared<untimed_setting::basic_predicate_gt<input_t>>(
          name, c);
    } else if (op == "ge" or op == "geq" or op == ">=") {
      result = std::make_shared<untimed_setting::basic_predicate_ge<input_t>>(
          name, c);
    } else {
      throw std::invalid_argument(
          "Unsupported predicate operator for the discrete timed setting");
    }

    return result;
  }

  static state_ptr_t make_predicate(const function_t& fn) {
    return std::make_shared<untimed_setting::predicate<input_t>>(fn);
  }

  static node_ptr_t make_node(const std::string& name,
                              const std::vector<node_ptr_t>& args) {
    node_ptr_t result;

    if (name == "disjunction") {
      result = std::make_shared<untimed_setting::disjunction<input_t>>(args);
    } else if (name == "conjunction") {
      result = std::make_shared<untimed_setting::conjunction<input_t>>(args);
    } else if (name == "negation") {
      result = std::make_shared<untimed_setting::negation<input_t>>(args);
    } else if (name == "implication") {
      result = std::make_shared<untimed_setting::implication<input_t>>(args);
    } else if (name == "past_sometime") {
      result = std::make_shared<untimed_setting::past_sometime<input_t>>(args);
    } else if (name == "past_always") {
      result = std::make_shared<untimed_setting::past_always<input_t>>(args);
    } else if (name == "since") {
      result = std::make_shared<untimed_setting::since<input_t>>(args);
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
      result = std::make_shared<untimed_setting::past_sometime<input_t>>(args);
    } else if (name == "past_always") {
      result = std::make_shared<untimed_setting::past_always<input_t>>(args);
    } else if (name == "previous") {
      result = std::make_shared<untimed_setting::previous<input_t>>(args);
    } else if (name == "since") {
      result = std::make_shared<untimed_setting::since<input_t>>(args);
    } else {
      throw std::invalid_argument(
          "Unsupported operator for the untimed setting");
    }

    return result;
  }
};

}  // namespace untimed_setting
}  // namespace reelay
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

#include "reelay/common.hpp"
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
  using time_t = int;
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

  explicit factory() {}

  static node_ptr_t make_node(const std::string &name, const kwargs &kw) {

    node_ptr_t result;

    if (name == "disjunction") {
      result = std::make_shared<disjunction<input_t>>(kw);
    } else if (name == "conjunction") {
      result = std::make_shared<conjunction<input_t>>(kw);
    } else if (name == "negation") {
      result = std::make_shared<negation<input_t>>(kw);
    } else if (name == "implication") {
      result = std::make_shared<implication<input_t>>(kw);
    } else {
      throw std::invalid_argument(
          "Unsupported operator for the untimed setting");
    }
    return result;
  }

  static state_ptr_t make_state(const std::string &name, const kwargs &kw) {

    state_ptr_t result;

    if (name == "proposition") {
      result = std::make_shared<proposition<input_t>>(kw);
    } else if(name == "lt" or name == "<") {
      result = std::make_shared<basic_predicate_lt<input_t>>(kw);
    } else if (name == "le" or name == "leq" or name == "<=") {
      result = std::make_shared<basic_predicate_le<input_t>>(kw);
    } else if (name == "gt" or name == ">") {
      result = std::make_shared<basic_predicate_gt<input_t>>(kw);
    } else if (name == "ge" or name == "geq" or name == ">=") {
      result = std::make_shared<basic_predicate_ge<input_t>>(kw);
    } else if (name == "previous") {
      result = std::make_shared<previous<input_t>>(kw);
    } else if (name == "past_sometime") {
      result = std::make_shared<past_sometime<input_t>>(kw);
    } else if (name == "past_always") {
      result = std::make_shared<past_always<input_t>>(kw);
    } else if (name == "since") {
      result = std::make_shared<since<input_t>>(kw);
    } else if (name == "predicate") {
      result = std::make_shared<predicate<input_t>>(kw);
    } else {
      throw std::invalid_argument(
          "Unsupported operator for the untimed setting");
    }
    return result;
  }

};

}  // namespace untimed_setting
}  // namespace reelay
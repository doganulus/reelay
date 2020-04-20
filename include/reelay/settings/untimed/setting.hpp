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

#include "reelay/settings/untimed/atomic_custom.hpp"
#include "reelay/settings/untimed/atomic_any.hpp"
#include "reelay/settings/untimed/atomic_false.hpp"
#include "reelay/settings/untimed/atomic_ge.hpp"
#include "reelay/settings/untimed/atomic_gt.hpp"
#include "reelay/settings/untimed/atomic_le.hpp"
#include "reelay/settings/untimed/atomic_lt.hpp"
#include "reelay/settings/untimed/atomic_ne.hpp"
#include "reelay/settings/untimed/atomic_number.hpp"
#include "reelay/settings/untimed/atomic_prop.hpp"
#include "reelay/settings/untimed/atomic_string.hpp"
#include "reelay/settings/untimed/atomic_true.hpp"

#include "reelay/settings/untimed/atomic_map.hpp"
#include "reelay/settings/untimed/atomic_nested.hpp"
#include "reelay/settings/untimed/atomic_nested_all.hpp"
#include "reelay/settings/untimed/atomic_nested_any.hpp"

#include "reelay/settings/untimed/conjunction.hpp"
#include "reelay/settings/untimed/disjunction.hpp"
#include "reelay/settings/untimed/implication.hpp"
#include "reelay/settings/untimed/negation.hpp"


#include "reelay/settings/untimed/past_always.hpp"
#include "reelay/settings/untimed/past_sometime.hpp"
#include "reelay/settings/untimed/previous.hpp"
#include "reelay/settings/untimed/since.hpp"

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

    if (name == "atomic_map") {
      result = std::make_shared<atomic_map<input_t>>(kw);
    } else if (name == "atomic_nested") {
      result = std::make_shared<atomic_nested<input_t>>(kw);
    // } else if (name == "atomic_nested_all") {
    //   result = std::make_shared<atomic_nested_all<input_t>>(kw);
    // } else if (name == "atomic_nested_any") {
    //   result = std::make_shared<atomic_nested_any<input_t>>(kw);
    } else if (name == "mapping_prop") {
      result = std::make_shared<atomic_prop<input_t>>(kw);
    } else if (name == "mapping_false") {
      result = std::make_shared<atomic_false<input_t>>(kw);
    } else if (name == "mapping_true") {
      result = std::make_shared<atomic_true<input_t>>(kw);
    } else if (name == "mapping_string") {
      result = std::make_shared<atomic_string<input_t>>(kw);
    } else if (name == "mapping_number") {
      result = std::make_shared<atomic_number<input_t>>(kw);
    } else if (name == "mapping_eq") {
      result = std::make_shared<atomic_number<input_t>>(kw);
    } else if (name == "mapping_ne") {
      result = std::make_shared<atomic_ne<input_t>>(kw);
    } else if (name == "mapping_ge") {
      result = std::make_shared<atomic_ge<input_t>>(kw);
    } else if (name == "mapping_gt") {
      result = std::make_shared<atomic_gt<input_t>>(kw);
    } else if (name == "mapping_le") {
      result = std::make_shared<atomic_le<input_t>>(kw);
    } else if (name == "mapping_lt") {
      result = std::make_shared<atomic_lt<input_t>>(kw);
    } else if (name == "mapping_any") {
      result = std::make_shared<atomic_any<input_t>>(kw);
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
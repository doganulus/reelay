/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "vector"

#include "reelay/networks/basic_structure.hpp"

namespace reelay {
namespace untimed_robustness_setting {

template <typename X, typename V>
struct past_always : public untimed_state<X, V> {
  using input_t = X;
  using output_t = V;

  using node_t = untimed_node<output_t>;
  using node_ptr_t = std::shared_ptr<node_t>;

  output_t value = std::numeric_limits<output_t>::max();

  node_ptr_t first;

  explicit past_always(const std::vector<node_ptr_t> &args) : first(args[0]) {}

  void update(const input_t &args) { value = std::min(value, first->output()); }

  output_t output() { return value; }
};

} // namespace untimed_robustness_setting
}  // namespace reelay
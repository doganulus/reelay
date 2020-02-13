/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "reelay/common.hpp"
#include "reelay/networks/basic_structure.hpp"

namespace reelay {
namespace discrete_timed_robustness_setting {

template <typename X, typename V, typename T>
struct since : public discrete_timed_state<X, V, T> {
  using time_t = T;
  using input_t = X;
  using output_t = V;

  using node_t = discrete_timed_node<output_t, time_t>;
  using state_t = discrete_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;


  node_ptr_t first;
  node_ptr_t second;

  output_t value = -reelay::infinity<output_t>::value();

  explicit since(const std::vector<node_ptr_t> &args)
      : first(args[0]), second(args[1]) {}

  explicit since(const kwargs &kw)
      : since(reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}

  void update(const input_t &, time_t now) override {
    value = std::max(second->output(now), std::min(value, first->output(now)));
  }

  output_t output(time_t) override { return value; }
};

}  // namespace discrete_timed_robustness_setting
}  // namespace reelay
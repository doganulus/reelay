/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "string"

#include "reelay/common.hpp"
#include "reelay/networks/basic_structure.hpp"

namespace reelay {
namespace discrete_timed_robustness_setting {

template <typename X, typename V, typename T>
struct atomic_map final : public discrete_timed_state<X, V, T> {
  using time_t = T;
  using input_t = X;
  using output_t = V;

  using node_t = discrete_timed_node<output_t, time_t>;
  using state_t = discrete_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  output_t value = -reelay::infinity<output_t>::value();

  std::vector<node_ptr_t> mappings;

  explicit atomic_map(const std::vector<node_ptr_t> &nodeptrs)
      : mappings(nodeptrs) {}

  explicit atomic_map(const kwargs &kw)
      : atomic_map(reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}

  void update(const input_t &args, time_t now) override {
    value = mappings[0]->output(now);
    for (std::size_t i = 1; i < mappings.size(); i++) {
      value = std::min(value, mappings[i]->output(now));
    }
  }

  output_t output(time_t) override { return value; }
};

} // namespace discrete_timed_robustness_setting
} // namespace reelay
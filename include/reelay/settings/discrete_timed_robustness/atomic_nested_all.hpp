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
struct atomic_nested_all final : public discrete_timed_state<X, V, T> {
  using time_t = T;
  using input_t = X;
  using output_t = V;

  using node_t = discrete_timed_node<output_t, time_t>;
  using state_t = discrete_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  output_t value = reelay::infinity<output_t>::value();

  std::vector<std::string> path;
  std::vector<state_ptr_t> states;

  explicit atomic_nested_all(const std::vector<std::string> &p,
                             const std::vector<state_ptr_t> &ptrs)
      : path(p), states(ptrs) {}

  explicit atomic_nested_all(const kwargs &kw)
      : atomic_nested_all(
            reelay::any_cast<std::vector<std::string>>(kw.at("path")),
            reelay::any_cast<std::vector<state_ptr_t>>(kw.at("args"))) {}

  void update(const input_t &args, time_t now) override {
    input_t deep_args = args;
    for (const auto &key : this->path) {
      if (not datafield<input_t>::contains(deep_args, key)) {
        return; // Do nothing if the key does not exist
      }
      deep_args = datafield<input_t>::at(deep_args, key);
    }

    value = reelay::infinity<output_t>::value();
    for (const auto &object_args : deep_args) {
      // std::cout << object_args << std::endl;
      for (const auto &state : this->states) {
        state->update(object_args, now);
      }
      output_t tempvalue = states[0]->output(now);
      for (std::size_t i = 1; i < states.size(); i++) {
        tempvalue = std::min(value, states[i]->output(now));
      }
      value = std::min(value, tempvalue);
    }
  }

  output_t output(time_t) override { return value; }
};

} // namespace discrete_timed_setting
} // namespace reelay
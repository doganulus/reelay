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
namespace discrete_timed_setting {

template <typename X, typename T>
struct atomic_nested final : public discrete_timed_state<X, bool, T> {
  using time_t = T;
  using input_t = X;
  using output_t = bool;

  using node_t = discrete_timed_node<output_t, time_t>;
  using state_t = discrete_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  output_t value = false;

  std::vector<std::string> path;
  std::vector<state_ptr_t> states;

  explicit atomic_nested(const std::vector<std::string> &p,
                         const std::vector<state_ptr_t> &ptrs)
      : path(p), states(ptrs) {}

  explicit atomic_nested(const kwargs &kw)
      : atomic_nested(
            reelay::any_cast<std::vector<std::string>>(kw.at("path")),
            reelay::any_cast<std::vector<state_ptr_t>>(kw.at("args"))) {}

  void update(const input_t &args, time_t now) override {

    input_t deep_args = args;
    for (const auto &key : this->path){
      if (not datafield<input_t>::contains(deep_args, key)) {
        return; // Do nothing if the key does not exist 
      }
      deep_args = datafield<input_t>::at(deep_args, key);
    }
    for (const auto &state : this->states) {
      state->update(deep_args, now);
    }
    value = std::all_of(states.cbegin(), states.cend(),
                        [now](state_ptr_t arg) { return arg->output(now); });
  }

  output_t output(time_t) override { return value; }
};

} // namespace discrete_timed_setting
} // namespace reelay
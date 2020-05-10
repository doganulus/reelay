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
namespace untimed_setting {

template <typename X>
struct atomic_nested final : public untimed_state<X, bool> {
  using input_t = X;
  using output_t = bool;

  using node_t = untimed_node<output_t>;
  using state_t = untimed_state<input_t, output_t>;

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

  void update(const input_t &args) override {

    input_t deep_args = args;
    for (const auto &key : this->path){
      if (not datafield<input_t>::contains(deep_args, key)) {
        return; // Do nothing if the key does not exist 
      }
      deep_args = datafield<input_t>::at(deep_args, key);
    }
    for (const auto &state : this->states) {
      state->update(deep_args);
    }
    value = std::all_of(states.cbegin(), states.cend(),
                        [](state_ptr_t arg) { return arg->output(); });
  }

  output_t output() override { return value; }
};

} // namespace discrete_timed_setting
} // namespace reelay
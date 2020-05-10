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
namespace dense_timed_setting {

template <typename X, typename T>
struct atomic_nested final : public dense_timed_state<X, interval_set<T>, T> {
  using time_t = T;
  using input_t = X;
  using output_t = reelay::interval_set<time_t>;

  using interval = reelay::interval<time_t>;
  using interval_set = reelay::interval_set<time_t>;

  using node_t = reelay::dense_timed_node<output_t, time_t>;
  using state_t = reelay::dense_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  output_t value = interval_set();

  std::vector<std::string> path;
  std::vector<state_ptr_t> states;

  explicit atomic_nested(const std::vector<std::string> &p,
                         const std::vector<state_ptr_t> &ptrs)
      : path(p), states(ptrs) {}

  explicit atomic_nested(const kwargs &kw)
      : atomic_nested(
            reelay::any_cast<std::vector<std::string>>(kw.at("path")),
            reelay::any_cast<std::vector<state_ptr_t>>(kw.at("args"))) {}

  void update(const input_t &pargs, const input_t &args, time_t previous,
              time_t now) override {

    bool updatable = true;
    input_t deep_args = args;
    for (const auto &key : this->path) {
      if (not datafield<input_t>::contains(deep_args, key)) {
        updatable = false;
        break; // Do nothing if the key does not exist
      }
      deep_args = datafield<input_t>::at(deep_args, key);
    }

    if (updatable) {
      for (const auto &state : this->states) {
        state->update(deep_args, deep_args, previous, now);
      }
    }
    value = states[0]->output(previous, now);
    for (size_t i = 1; i < states.size(); i++) {
      value = value & states[i]->output(previous, now);
    }
  }

  output_t output(time_t, time_t) override { return value; }
};

} // namespace discrete_timed_setting
} // namespace reelay
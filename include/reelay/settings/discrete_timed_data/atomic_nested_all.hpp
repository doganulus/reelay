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
#include "reelay/unordered_data.hpp"

namespace reelay {
namespace discrete_timed_data_setting {

template <typename X, typename T>
struct atomic_nested_all final : public discrete_timed_state<X, data_set_t, T> {
  using time_t = T;
  using input_t = X;
  using value_t = data_set_t;
  using output_t = data_set_t;

  using node_t = discrete_timed_node<output_t, time_t>;
  using state_t = discrete_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  using function_t = std::function<data_set_t(const input_t &)>;

  data_mgr_t manager;
  data_set_t value;

  std::vector<std::string> path;
  std::vector<state_ptr_t> states;

  explicit atomic_nested_all(const data_mgr_t &mgr,
                             const std::vector<std::string> &p,
                             const std::vector<state_ptr_t> &ptrs)
      : manager(mgr), path(p), states(ptrs) {}

  explicit atomic_nested_all(const kwargs &kw)
      : atomic_nested_all(
            reelay::any_cast<data_mgr_t>(kw.at("manager")),
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

    value = manager->one();
    for (const auto &object_args : deep_args) {
      // std::cout << object_args << std::endl;
      for (const auto &state : this->states) {
        state->update(object_args, now);
      }
      output_t tempvalue = states[0]->output(now);
      for (std::size_t i = 1; i < states.size(); i++) {
        tempvalue *= states[i]->output(now);
      }
      value = value * tempvalue;
    }
  }

  output_t output(time_t) override { return value; }
};

} // namespace discrete_timed_setting
} // namespace reelay
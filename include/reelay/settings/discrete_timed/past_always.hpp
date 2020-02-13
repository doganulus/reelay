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
namespace discrete_timed_setting {

template <typename X, typename T>
struct past_always : public discrete_timed_state<X, bool, T> {
  using time_t = T;
  using input_t = X;
  using output_t = bool;

  using node_t = discrete_timed_node<output_t, time_t>;
  using state_t = discrete_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  bool value = true;

  node_ptr_t first;

  explicit past_always(const std::vector<node_ptr_t> &args) : first(args[0]) {}

  explicit past_always(const kwargs &kw)
      : past_always(reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}

  void update(const input_t&, time_t now) override {
    value = first->output(now) && value;
  }

  output_t output(time_t) override { return value; }
};

}  // namespace discrete_timed_setting
}  // namespace reelay
/*
 * Copyright (c) 2019-2023 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "memory"
#include "vector"

#include "reelay/common.hpp"
#include "reelay/networks/basic_structure.hpp"

namespace reelay {
namespace discrete_timed_setting {

template <typename X, typename T>
struct conjunction final : public discrete_timed_node<bool, T> {
  using time_t = T;
  using input_t = X;
  using output_t = bool;

  using node_t = discrete_timed_node<output_t, time_t>;
  using state_t = discrete_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  std::vector<node_ptr_t> args;

  explicit conjunction(const std::vector<node_ptr_t> &nodeptrs)
      : args(nodeptrs) {}

  explicit conjunction(const kwargs &kw)
      : conjunction(reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}

  output_t output(time_t now) {
    return std::all_of(args.cbegin(), args.cend(),
                       [now](node_ptr_t arg) { return arg->output(now); });
  }
};

}  // namespace discrete_timed_setting
}  // namespace reelay
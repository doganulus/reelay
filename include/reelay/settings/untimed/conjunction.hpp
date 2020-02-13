/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "vector"

#include "reelay/common.hpp"
#include "reelay/networks/basic_structure.hpp"

namespace reelay {
namespace untimed_setting {

template <typename X>
struct conjunction : public untimed_node<bool> {
  using input_t = X;
  using output_t = bool;

  using node_t = untimed_node<output_t>;
  using node_ptr_t = std::shared_ptr<node_t>;

  std::vector<node_ptr_t> args;

  explicit conjunction(const std::vector<node_ptr_t> &args)
      : args(args) {}

  explicit conjunction(const kwargs &kw)
      : conjunction(reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}

  output_t output() {
    return std::all_of(args.cbegin(), args.cend(),
                       [](node_ptr_t arg) { return arg->output(); });
  }
};

}  // namespace untimed_setting
}  // namespace reelay
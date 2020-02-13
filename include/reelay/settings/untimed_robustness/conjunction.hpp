/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "vector"

#include "reelay/networks/basic_structure.hpp"

namespace reelay {
namespace untimed_robustness_setting {

template <typename X, typename V>
struct conjunction : public untimed_node<V> {
  using input_t = X;
  using output_t = V;

  using node_t = untimed_node<output_t>;
  using node_ptr_t = std::shared_ptr<node_t>;

  std::vector<node_ptr_t> args;

  explicit conjunction(const std::vector<node_ptr_t> &nodeptrs)
      : args(nodeptrs) {}

  explicit conjunction(const kwargs &kw)
      : conjunction(reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}
      
  output_t output() {
    output_t result = args[0]->output();
    for (size_t i = 1; i < args.size(); i++) {
      result = std::min(result, args[i]->output());
    }
    return result;
  }
};

} // namespace untimed_robustness_setting
} // namespace reelay
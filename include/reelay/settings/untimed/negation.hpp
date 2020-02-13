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
struct negation : public untimed_node<bool> {
  using input_t = X;
  using output_t = bool;

  using Node = untimed_node<output_t>;
  using node_ptr_t = std::shared_ptr<Node>;

  node_ptr_t arg1;

  explicit negation(const std::vector<node_ptr_t> &args) : arg1(args[0]) {}

  explicit negation(const kwargs &kw)
      : negation(reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}

  output_t output() { return not arg1->output(); }
};

}  // namespace untimed_setting
}  // namespace reelay
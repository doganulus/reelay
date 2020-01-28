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
struct past_always : public untimed_state<X, bool> {
  using input_t = X;
  using output_t = bool;

  using node_t = untimed_node<output_t>;
  using node_ptr_t = std::shared_ptr<node_t>;

  node_ptr_t first;

  bool value = true;

  explicit past_always(const std::vector<node_ptr_t> &args) : first(args[0]) {}

  explicit past_always(const kwargs &kw)
      : past_always(reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}

  void update(const input_t&) { value = first->output() && value; }

  output_t output() { return value; }
};

}  // namespace untimed_setting
}  // namespace reelay
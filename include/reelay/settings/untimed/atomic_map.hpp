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
struct atomic_map final : public untimed_state<X, bool> {
  using input_t = X;
  using output_t = bool;

  using node_t = untimed_node<output_t>;
  using node_ptr_t = std::shared_ptr<node_t>;

  output_t value = false;

  std::vector<node_ptr_t> mappings;

  explicit atomic_map(const std::vector<node_ptr_t> &nodeptrs)
      : mappings(nodeptrs) {}

  explicit atomic_map(const kwargs &kw)
      : atomic_map(reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}

  void update(const input_t &args) override {
    value = std::all_of(mappings.cbegin(), mappings.cend(),
                        [](node_ptr_t arg) { return arg->output(); });
  }

  output_t output() override {
    return value;
  }
};

} // namespace untimed_setting
} // namespace reelay
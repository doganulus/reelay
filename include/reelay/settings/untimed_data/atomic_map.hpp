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
namespace untimed_data_setting {

template <typename X>
struct atomic_map final : public untimed_state<X, data_set_t> {
  using input_t = X;
  using output_t = data_set_t;

  using node_t = untimed_node<output_t>;
  using node_ptr_t = std::shared_ptr<node_t>;

  using function_t = std::function<data_set_t(const input_t &)>;

  data_mgr_t manager;
  data_set_t value;

  std::vector<node_ptr_t> mappings;

  explicit atomic_map(const data_mgr_t &mgr,
                      const std::vector<node_ptr_t> &nodeptrs)
      : manager(mgr), value(mgr->zero()), mappings(nodeptrs) {}

  explicit atomic_map(const kwargs &kw)
      : atomic_map(reelay::any_cast<data_mgr_t>(kw.at("manager")),
                   reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}

  void update(const input_t &args) override {
    value = mappings[0]->output();
    for (std::size_t i = 1; i < mappings.size(); i++) {
      value *= mappings[i]->output();
    }
  }

  output_t output() override {
    return value;
  }
};

} // namespace untimed_data_setting
} // namespace reelay
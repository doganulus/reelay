/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "reelay/common.hpp"
#include "reelay/datafield.hpp"
#include "reelay/networks/basic_structure.hpp"
#include "reelay/unordered_data.hpp"

namespace reelay {
namespace discrete_timed_data_setting {

template <typename X, typename T>
struct atomic_list final : public discrete_timed_state<X, data_set_t, T> {
  using time_t = T;
  using input_t = X;
  using value_t = data_set_t;
  using output_t = data_set_t;

  using node_t = discrete_timed_node<output_t, time_t>;
  using node_ptr_t = std::shared_ptr<node_t>;

  data_mgr_t manager;
  data_set_t value;

  std::vector<node_ptr_t> listings;

  explicit atomic_list(const data_mgr_t &mgr,
                       const std::vector<node_ptr_t> &nodeptrs)
      : manager(mgr), value(mgr->zero()), listings(nodeptrs) {}

  explicit atomic_list(const kwargs &kw)
      : atomic_list(reelay::any_cast<data_mgr_t>(kw.at("manager")),
                    reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}

  void update(const input_t &args, time_t now) override {
    if(args.size() != listings.size()){
      value = manager->zero();
      return;
    } else {
      value = listings[0]->output(now);
      for (std::size_t i = 1; i < listings.size(); i++) {
        value *= listings[i]->output(now);
      }
    }
  }

  output_t output(time_t) override { return value; }
};

} // namespace untimed_data_setting
} // namespace reelay
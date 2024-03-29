/*
 * Copyright (c) 2019-2023 Dogan Ulus
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
#include "reelay/intervals.hpp"
#include "reelay/networks/basic_structure.hpp"
#include "reelay/unordered_data.hpp"

namespace reelay {
namespace dense_timed_data_setting {

template <typename X, typename T>
struct atomic_map final : public dense_timed_state<X, data_interval_map<T>, T> {
  using time_t = T;
  using input_t = X;
  using value_t = data_set_t;
  using output_t = reelay::data_interval_map<time_t>;

  using interval = reelay::interval<time_t>;
  using interval_map = reelay::data_interval_map<time_t>;

  using node_t = dense_timed_node<output_t, time_t>;
  using state_t = dense_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  data_mgr_t manager;

  interval_map value;

  std::vector<node_ptr_t> mappings;

  explicit atomic_map(const data_mgr_t &mgr,
                      const std::vector<node_ptr_t> &nodeptrs)
      : manager(mgr), mappings(nodeptrs) {}

  explicit atomic_map(const kwargs &kw)
      : atomic_map(reelay::any_cast<data_mgr_t>(kw.at("manager")),
                   reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}

  void update(const input_t &, const input_t &, time_t previous,
              time_t now) override {
    value = mappings[0]->output(previous, now);
    for (size_t i = 1; i < mappings.size(); i++) {
      value = value - mappings[i]->output(previous, now);
    }
  }

  output_t output(time_t previous, time_t now) override {
    return (value - interval::left_open(0, previous)) -
           interval::left_open(now, infinity<time_t>::value());
  }
};

} // namespace dense_timed_setting
} // namespace reelay
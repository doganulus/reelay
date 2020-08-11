/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>
#include <vector>

#include "reelay/common.hpp"
#include "reelay/intervals.hpp"
#include "reelay/networks/basic_structure.hpp"
#include "reelay/unordered_data.hpp"

namespace reelay {
namespace dense_timed_data_setting {

template <typename X, typename T>
struct implication final : public dense_timed_node<data_interval_map<T>, T> {
  using time_t = T;
  using input_t = X;
  using value_t = data_set_t;
  using output_t = reelay::data_interval_map<time_t>;

  using node_t = dense_timed_node<output_t, time_t>;
  using state_t = dense_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  using interval = reelay::interval<time_t>;
  using interval_map = reelay::data_interval_map<time_t>;

  node_ptr_t arg1;
  node_ptr_t arg2;

  explicit implication(const std::vector<node_ptr_t> &args)
      : arg1(args[0]), arg2(args[1]) {}

  explicit implication(const kwargs &kw)
      : implication(reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}

  output_t output(time_t previous, time_t now) {
    auto result = interval_map();
    for (const auto &intv : arg1->output(previous, now)) {
      result.add(std::make_pair(intv.first, ~intv.second));
    }
    for (const auto &intv : arg2->output(previous, now)) {
      result.add(intv);
    }
    return result;
  }
};

} // namespace dense_timed_data_setting
} // namespace reelay
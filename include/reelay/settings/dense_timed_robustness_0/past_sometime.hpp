/*
 * Copyright (c) 2019-2023 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "vector"

#include "reelay/intervals.hpp"
#include "reelay/networks/basic_structure.hpp"

namespace reelay {
namespace dense_timed_robustness_0_setting {

template <typename X, typename V, typename T>
struct past_sometime final
    : public dense_timed_state<X, robustness_interval_map<T, V>, T> {
  using time_t = T;
  using input_t = X;
  using value_t = V;
  using output_t = reelay::robustness_interval_map<time_t, value_t>;

  using interval = reelay::interval<time_t>;
  using interval_map = reelay::robustness_interval_map<time_t, value_t>;

  using node_t = dense_timed_node<output_t, time_t>;
  using state_t = dense_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  node_ptr_t first;

  interval_map value = interval_map();

  explicit past_sometime(const std::vector<node_ptr_t> &args)
      : first(args[0]) {}

  explicit past_sometime(const kwargs &kw)
      : past_sometime(reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}

  void update(const input_t &, const input_t &, time_t previous,
              time_t now) override {
    for (const auto &intv : first->output(previous, now)) {
      value.add(std::make_pair(
          interval::left_open(intv.first.lower(), infinity<time_t>::value()),
          intv.second));
    }
    value = value & interval::left_open(previous, infinity<time_t>::value());
  }

  output_t output(time_t previous, time_t now) override {
    return value & interval::left_open(previous, now);
  }
};

}  // namespace dense_timed_setting
}  // namespace reelay
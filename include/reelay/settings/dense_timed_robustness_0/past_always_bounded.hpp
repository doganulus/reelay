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
struct past_always_bounded final
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
  
  time_t lbound;
  time_t ubound;

  interval_map value;

  past_always_bounded(const std::vector<node_ptr_t> &args, time_t l = 0,
                      time_t u = 0)
      : first(args[0]), lbound(l), ubound(u) {
    value = interval_map(
        std::make_pair(interval::left_open(-infinity<time_t>::value(), lbound),
                       -infinity<value_t>::value()));
      }

  explicit past_always_bounded(const kwargs &kw)
      : past_always_bounded(
            reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args")),
            reelay::any_cast<time_t>(kw.at("lbound")),
            reelay::any_cast<time_t>(kw.at("ubound"))) {}

  void update(const input_t &, const input_t &, time_t previous,
              time_t now) override {
    for (const auto &intv : first->output(previous, now)) {
      value.add(std::make_pair(interval::left_open(intv.first.lower() + lbound,
                                                   intv.first.upper() + ubound),
                               -intv.second));
    }
    value = value & interval::left_open(previous, infinity<time_t>::value());
  }

  output_t output(time_t previous, time_t now) override {
    auto negated_value = interval_map();
    for (const auto &intv : this->value) {
      negated_value.add(std::make_pair(intv.first, -intv.second));
    }
    return negated_value & interval::left_open(previous, now);
  }
};

}  // namespace dense_timed_setting
}  // namespace reelay
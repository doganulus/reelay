/*
 * Copyright (c) 2019-2020 Dogan Ulus
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
namespace dense_timed_setting {

template <typename X, typename T>
struct past_sometime_bounded_half
    : public dense_timed_state<X, interval_set<T>, T> {
  using time_t = T;
  using input_t = X;
  using output_t = reelay::interval_set<time_t>;

  using node_t = dense_timed_node<output_t, time_t>;
  using node_ptr_t = std::shared_ptr<node_t>;

  using interval = reelay::interval<time_t>;
  using interval_set = reelay::interval_set<time_t>;

  interval_set value = interval_set();

  node_ptr_t first;
  time_t lbound;

  past_sometime_bounded_half(const std::vector<node_ptr_t> &args, time_t l=0)
      : first(args[0]), lbound(l) {}

  explicit past_sometime_bounded_half(const kwargs &kw)
      : past_sometime_bounded_half(
            reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args")),
            reelay::any_cast<time_t>(kw.at("lbound"))) {}

  void update(const input_t&,
              const input_t&,
              time_t previous,
              time_t now) override {
    for (const auto& intv : first->output(previous, now)) {
      value.add(interval::left_open(intv.lower() + lbound,
                                    std::numeric_limits<time_t>::max()));
    }
  }

  output_t output(time_t previous, time_t now) override {
    return value & interval::left_open(previous, now);
  }
};

}  // namespace dense_timed_setting
}  // namespace reelay
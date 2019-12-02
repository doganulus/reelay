/*
 * Copyright (c) 2019 Dogan Ulus
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
struct past_sometime_bounded : public dense_timed_state<X, interval_set<T>, T> {
  using time_t = T;
  using input_t = X;
  using output_t = reelay::interval_set<time_t>;

  using node_t = dense_timed_node<output_t, time_t>;
  using node_ptr_t = std::shared_ptr<node_t>;

  using interval = reelay::interval<time_t>;
  using interval_set = reelay::interval_set<time_t>;

  interval_set value = interval_set();

  time_t lbound = 0;
  time_t ubound = 0;

  node_ptr_t first;

  past_sometime_bounded(const std::vector<node_ptr_t> &args, time_t l, time_t u)
      : first(args[0]), lbound(l), ubound(u) {}

  void update(const input_t& pargs,
              const input_t& args,
              time_t previous,
              time_t now) override {
    for (const auto& intv : first->output(previous, now)) {
      value.add(
          interval::left_open(intv.lower() + lbound, intv.upper() + ubound));
    }
  }

  output_t output(time_t previous, time_t now) override {
    return value & interval::left_open(previous, now);
  }
};

}  // namespace dense_timed_setting
}  // namespace reelay
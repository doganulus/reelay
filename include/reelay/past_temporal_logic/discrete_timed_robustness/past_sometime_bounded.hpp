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
namespace discrete_timed_robustness_setting {

template <typename X, typename V, typename T>
struct past_sometime_bounded : public discrete_timed_state<X, V, T> {
  using time_t = T;
  using input_t = X;
  using output_t = V;

  using node_t = discrete_timed_node<output_t, time_t>;
  using state_t = discrete_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  using interval = reelay::interval<time_t>;
  using interval_map = reelay::robustness_interval_map<time_t, output_t>;

  interval_map value = interval_map();

  node_ptr_t first;

  time_t lbound = 0;
  time_t ubound = 0;

  past_sometime_bounded(const std::vector<node_ptr_t> &args, time_t l, time_t u)
      : first(args[0]), lbound(l), ubound(u) {
    value.add(
        std::make_pair(interval::closed(
          -reelay::infinity<time_t>::value(),lbound),
          -reelay::infinity<output_t>::value()));
      }

  void update(const input_t &args, time_t now) {
    value.add(std::make_pair(
        reelay::interval<time_t>::closed(now + lbound, now + ubound),
        first->output(now)));
    value = value -
            interval_map(std::make_pair(
                reelay::interval<time_t>::right_open(0, now),
                -reelay::infinity<output_t>::value()));
  }

  output_t output(time_t now) { return value(now); }
};

} // namespace discrete_timed_robustness_setting
}  // namespace reelay
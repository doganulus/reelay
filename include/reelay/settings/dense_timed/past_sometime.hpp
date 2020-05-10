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
struct past_sometime final : public dense_timed_state<X, interval_set<T>, T> {
  using time_t = T;
  using input_t = X;
  using output_t = reelay::interval_set<time_t>;

  using node_t = dense_timed_node<output_t, time_t>;
  using node_ptr_t = std::shared_ptr<node_t>;

  using interval = reelay::interval<time_t>;
  using interval_set = reelay::interval_set<time_t>;

  bool done = false;
  interval_set value = interval_set();

  node_ptr_t first;

  explicit past_sometime(const std::vector<node_ptr_t> &args)
      : first(args[0]) {}

  explicit past_sometime(const kwargs &kw)
      : past_sometime(reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}

  void update(const input_t&,
              const input_t&,
              time_t previous,
              time_t now) override {
    if(done) {
      return;
    }
    if (first->output(previous, now).size() == 0){
      return;
    }
    time_t earliest_time = first->output(previous, now).begin()->lower();
    value = interval_set(
        interval::left_open(earliest_time, infinity<time_t>::value()));
    done = true;
  }

  output_t output(time_t previous, time_t now) override {
    return value & interval::left_open(previous, now);
  }
};

}  // namespace dense_timed_setting
}  // namespace reelay
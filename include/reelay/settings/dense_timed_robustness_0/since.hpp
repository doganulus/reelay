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
namespace dense_timed_robustness_0_setting {

template <typename X, typename V, typename T>
struct since final
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
  node_ptr_t second;

  interval_map value = interval_map();
  interval_map last_pair = interval_map(
      std::make_pair(interval::left_open(-infinity<time_t>::value(), 0),
                     -infinity<value_t>::value()));

  explicit since(const std::vector<node_ptr_t> &args)
      : first(args[0]), second(args[1]) {}

  explicit since(const kwargs &kw)
      : since(reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}

  void update(value_t p1, value_t p2, time_t previous, time_t now) {

    last_pair = interval_map(
        std::make_pair(interval::left_open(previous, now),
                       std::max(p2, std::min(last_pair.begin()->second, p1))));

    value = value | last_pair;
    value = value & interval::closed(previous, infinity<time_t>::value());
  }

  void update(const input_t&,
              const input_t&,
              time_t previous,
              time_t now) override {
    /*
     * The following code performs the sychronization between two arbitrary
     * chunks and calls the update function for each constant period
     * from past to future. The synchronization algorithm is a variant of 
     * plane sweep algorithms from computational geometry.
     */

    // Sweep line starts from the earliest timepoint in the segment
    time_t time = previous;

    auto active1 = first->output(previous, now);
    auto active2 = second->output(previous, now);

    auto it1 = active1.begin();
    auto it2 = active2.begin();

    while (it1 != active1.end() and it2 != active2.end()) {
      if (it1->first.upper() < it2->first.upper()) {
        update(it1->second, it2->second, time, it1->first.upper());
        time = it1->first.upper();
        it1++;

      } else if (it2->first.upper() < it1->first.upper()) {
        update(it1->second, it2->second, time, it2->first.upper());
        time = it2->first.upper();
        it2++;

      } else { // it1->first.upper() == it2->first.upper()
        update(it1->second, it2->second, time, it1->first.upper());
        time = it1->first.upper();
        it1++;
        it2++;
      }
    }
  }

  output_t output(time_t previous, time_t now) override {
    return value & interval::left_open(previous, now);
  }
};

}  // namespace dense_timed_setting
}  // namespace reelay
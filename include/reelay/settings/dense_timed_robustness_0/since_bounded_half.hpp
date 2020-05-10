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
struct since_bounded_half final
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

  time_t lbound;

  interval_map value0 = interval_map();
  interval_map value1 = interval_map();
  interval_map value2 = interval_map();

  interval_map last_pair0 = interval_map();
  interval_map last_pair1 = interval_map();
  interval_map last_pair2 = interval_map();

  since_bounded_half(const std::vector<node_ptr_t> &args, time_t l = 0)
      : first(args[0]), second(args[1]), lbound(l) {

    value1.add(std::make_pair(
        interval::closed(-reelay::infinity<time_t>::value(), lbound),
        -reelay::infinity<value_t>::value()));
    value2.add(std::make_pair(
        interval::closed(-reelay::infinity<time_t>::value(), lbound),
        -reelay::infinity<value_t>::value()));
  }

  explicit since_bounded_half(const kwargs &kw)
      : since_bounded_half(
            reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args")),
            reelay::any_cast<time_t>(kw.at("lbound"))) {}

  void update(value_t p1, value_t p2, time_t previous, time_t now) {

    last_pair0 = interval_map(
        std::make_pair(interval::left_open(previous, now),
                       std::max(p2, std::min(last_pair0.begin()->second, p1))));
    last_pair1 = interval_map(std::make_pair(
        reelay::interval<time_t>::left_open(previous, now + lbound),
        -last_pair0.begin()->second));
    last_pair2 = interval_map(std::make_pair(
        interval::left_open(previous + lbound, infinity<value_t>::value()),
        p2));

    value0 = value0 | last_pair0;
    value1 = value1 | last_pair1;
    value2 = value2 | last_pair2;
  }

  void update(const input_t &, const input_t &, time_t previous,
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

    value0 = value0 & interval::left_open(previous, infinity<time_t>::value());
    value1 = value1 & interval::left_open(previous, infinity<time_t>::value());
    value2 = value2 & interval::left_open(previous, infinity<time_t>::value());
  }

  output_t output(time_t previous, time_t now) override {
    auto negated_value1 = interval_map();
    for (const auto &intv : value1) {
      negated_value1.add(std::make_pair(intv.first, -intv.second));
    }
    return (negated_value1 & interval::left_open(previous, now)) -
           (value2 & interval::left_open(previous, now));
  }
};

}  // namespace dense_timed_setting
}  // namespace reelay
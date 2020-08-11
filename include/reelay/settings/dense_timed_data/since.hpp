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
struct since final : public dense_timed_state<X, data_interval_map<T>, T> {
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
  using interval_map_segment = typename interval_map::segment_type;

  data_mgr_t manager;

  node_ptr_t first;
  node_ptr_t second;

  interval_map value = interval_map();
  interval_map_segment last_pair;

  explicit since(const data_mgr_t &mgr, const std::vector<node_ptr_t> &args)
      : manager(mgr), first(args[0]), second(args[1]) {
    last_pair = std::make_pair(
        interval::left_open(-infinity<time_t>::value(), 0), manager->zero());
  }

  explicit since(const kwargs &kw)
      : since(reelay::any_cast<data_mgr_t>(kw.at("manager")),
              reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}

  void update(value_t p1, value_t p2, time_t previous, time_t now) {

    last_pair = std::make_pair(interval::left_open(previous, now),
                               p2 + (last_pair.second * p1));
    value.add(last_pair);
    value -= interval::closed(-infinity<time_t>::value(), previous);
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
    return value - interval::left_open(now, infinity<time_t>::value());
  }
};

}  // namespace dense_timed_setting
}  // namespace reelay
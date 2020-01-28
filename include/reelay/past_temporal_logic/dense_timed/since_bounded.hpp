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
struct since_bounded : public dense_timed_state<X, interval_set<T>, T> {
  using time_t = T;
  using input_t = X;
  using output_t = reelay::interval_set<time_t>;

  using node_t = dense_timed_node<output_t, time_t>;
  using node_ptr_t = std::shared_ptr<node_t>;

  using interval = reelay::interval<time_t>;
  using interval_set = reelay::interval_set<time_t>;

  interval_set value = interval_set();

  node_ptr_t first;
  node_ptr_t second;

  time_t lbound = 0;
  time_t ubound = 0;

  since_bounded(const std::vector<node_ptr_t> &args, time_t l, time_t u)
      : first(args[0]), second(args[1]), lbound(l), ubound(u) {}

  explicit since_bounded(const kwargs &kw)
      : since_bounded(reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args")),
                      reelay::any_cast<time_t>(kw.at("lbound")),
                      reelay::any_cast<time_t>(kw.at("ubound"))) {}

  void update(bool p1, bool p2, time_t previous, time_t now) {
    if (previous == now) {
      return;
    }

    if (p1 and p2) {
      value.add(interval::left_open(previous + lbound, now + ubound));

      // Comment out the line below for lazy trimming
      // state = state &
      // interval_set<T>(interval<T>::left_open(current.begin()->lower(),
      // std::numeric_limits<T>::max()));

    } else if (!p1 and p2) {
      value = value & interval::left_open(0, previous);
      value.add(interval::left_open(now + lbound, now + ubound));

    } else if (p1 and !p2) {
      // Comment out the line below for lazy trimming
      // state = state &
      // interval_set<T>(interval<T>::left_open(current.begin()->lower(),
      // std::numeric_limits<T>::max()));

    } else {
      value = value & interval::left_open(0, previous);
    }
  }

  void update(const input_t&,
              const input_t&,
              time_t previous,
              time_t now) override {
    /*
     *  The following code block performs the sychronization between two
     * arbitrary chunks and calls the _update function for each constant period
     * sequentially. The synchronization algorithm is a variant of the plane
     * sweep algorithm from computational geometry.
     */

    // Sweep line starts from the earliest timepoint in the segment
    time_t time = previous;

    // Local variables
    bool p1 = false;
    bool p2 = false;

    std::vector<T> bounds1;  // bounds1.reserve(p1set.size()*2+1);
    std::vector<T> bounds2;  // bounds2.reserve(p2set.size()*2+1);

    for (const auto& intv : first->output(previous, now)) {
      bounds1.push_back(intv.lower());
      bounds1.push_back(intv.upper());
    }

    for (const auto& intv : second->output(previous, now)) {
      bounds2.push_back(intv.lower());
      bounds2.push_back(intv.upper());
    }

    // Add the latest timepoint in the segment as the final bound
    bounds1.push_back(now);
    bounds2.push_back(now);

    auto it1 = bounds1.begin();
    auto it2 = bounds2.begin();

    value = value & interval_set(interval::left_open(
                        previous, std::numeric_limits<time_t>::max()));

    while (it1 != bounds1.end() and it2 != bounds2.end()) {
      if (*it1 < *it2) {
        // std::cout << time << ' '<< *it1 << '|' << p1 << p2 << std::endl;
        update(p1, p2, time, *it1);
        p1 = not p1;
        time = *it1;
        it1++;

      } else if (*it1 > *it2) {
        // std::cout << time << ' '<< *it2 << '|' << p1 << p2 << std::endl;
        update(p1, p2, time, *it2);
        p2 = not p2;
        time = *it2;
        it2++;

      } else {  // *it1 == *it2
        // std::cout << time << ' '<< *it1 << '|' << p1 << p2 << std::endl;
        update(p1, p2, time, *it1);
        p1 = not p1;
        p2 = not p2;
        time = *it1;
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
/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "functional"
#include "memory"
#include "vector"

#include "reelay/common.hpp"
#include "reelay/datafield.hpp"
#include "reelay/intervals.hpp"
#include "reelay/networks/basic_structure.hpp"

namespace reelay {
namespace dense_timed_setting {

template <typename X, typename T, typename K = std::string>
struct atomic_gt_1 final : public dense_timed_state<X, interval_set<T>, T> {
  using key_t = K;
  using input_t = X;
  using time_t = T;

  using base_t = reelay::interval_set<time_t>;
  using output_t = reelay::interval_set<time_t>;

  using function_t =
      std::function<output_t(const input_t &, const input_t &, time_t, time_t)>;

  using interval = reelay::interval<time_t>;
  using interval_set = reelay::interval_set<time_t>;

  interval_set value = interval_set();

  key_t key;
  double constant;

  time_t t0 = 0;
  double y0 = 0;

  explicit atomic_gt_1(const key_t &k, const std::string &c)
      : key(k), constant(std::stod(c)) {}

  explicit atomic_gt_1(const kwargs &kw)
      : atomic_gt_1(reelay::any_cast<key_t>(kw.at("key")),
                    reelay::any_cast<std::string>(kw.at("constant"))) {}

  void update(const input_t &pargs, const input_t &args, time_t previous,
              time_t now) override {

    double y1 = datafield<input_t>::as_floating(args, key);

    if (y0 < constant and y1 < constant) {
      value = interval_set();
    } else if (y0 > constant and y1 > constant) {
      value = interval_set(interval::left_open(t0, now));
    } else if (y0 > y1) {
      time_t crossing = now - (now - t0) * ((constant - y1) / (y0 - y1));
      value = interval_set(interval::left_open(t0, crossing));
    } else if (y0 < y1) {
      time_t crossing = t0 + (now - t0) * ((constant - y0) / (y1 - y0));
      value = interval_set(interval::left_open(crossing, now));
    } else { // y0 == y1 == c is true
      value = interval_set();
    }

    t0 = now;
    y0 = y1;
  }

  output_t output(time_t previous, time_t now) override {
    return value;
  }
};

} // namespace dense_timed_setting
} // namespace reelay
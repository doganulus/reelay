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
namespace dense_timed_robustness_0_setting {

template <typename X, typename V, typename T, typename K = std::string>
struct atomic_prop final
    : public dense_timed_state<X, robustness_interval_map<T, V>, T> {
  using key_t = K;
  using time_t = T;
  using input_t = X;
  using value_t = V;
  using output_t = reelay::robustness_interval_map<time_t, value_t>;

  using interval = reelay::interval<time_t>;
  using interval_map = reelay::robustness_interval_map<time_t, value_t>;

  interval_map value = interval_map(
      std::make_pair(interval::left_open(0, reelay::infinity<time_t>::value()),
                     -reelay::infinity<value_t>::value()));

  key_t key;

  explicit atomic_prop(const key_t &key) : key(key) {}

  explicit atomic_prop(const kwargs &kw)
      : atomic_prop(reelay::any_cast<key_t>(kw.at("key"))) {}

  void update(const input_t &pargs, const input_t &args, time_t previous,
              time_t now) override {

    if (not datafield<input_t>::contains(args, key)) {
      return; // Do nothing if the key does not exist - existing value persists
    }

    double new_data = datafield<input_t>::as_floating(args, key);

    value = value & interval::left_open(previous, now);
    value.add(std::make_pair(
        interval::left_open(now, reelay::infinity<time_t>::value()), new_data));
  }

  output_t output(time_t previous, time_t now) override {
    return value & interval::left_open(previous, now);
  }
};

} // namespace dense_timed_robustness_0_setting
} // namespace reelay
/*
 * Copyright (c) 2019-2023 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "reelay/common.hpp"
#include "reelay/datafield.hpp"
#include "reelay/intervals.hpp"
#include "reelay/networks/basic_structure.hpp"
#include "reelay/unordered_data.hpp"

namespace reelay {
namespace dense_timed_data_setting {

template <typename X, typename T, typename K = std::string>
struct atomic_any final : public dense_timed_state<X, data_interval_map<T>, T> {
  using key_t = K;
  using time_t = T;
  using input_t = X;
  using value_t = data_set_t;
  using output_t = reelay::data_interval_map<time_t>;

  using interval = reelay::interval<time_t>;
  using interval_map = reelay::data_interval_map<time_t>;

  data_mgr_t manager;

  interval_map value = interval_map(
      std::make_pair(interval::left_open(0, reelay::infinity<time_t>::value()),
                     manager->one()));

  key_t key;

  explicit atomic_any(const data_mgr_t &mgr, const key_t &key)
      : manager(mgr), key(key) {}

  explicit atomic_any(const kwargs &kw)
      : atomic_any(reelay::any_cast<data_mgr_t>(kw.at("manager")),
                   reelay::any_cast<key_t>(kw.at("key"))) {}

  void update(const input_t& pargs,
              const input_t& args,
              time_t previous,
              time_t now) override {

    bool key_exists = datafield<input_t>::contains(args, key);

    value = value - interval::left_open(0, previous);
    value = value - interval::left_open(now, infinity<time_t>::value());

    if (key_exists) {
      value.add(std::make_pair(
          interval::left_open(now, reelay::infinity<time_t>::value()),
          manager->one()));
    } else {
      value.add(std::make_pair(
          interval::left_open(now, reelay::infinity<time_t>::value()),
          manager->zero()));
    }
  }

  output_t output(time_t previous, time_t now) override {
    return (value - interval::left_open(0, previous)) -
           interval::left_open(now, infinity<time_t>::value());
  }
};

} // namespace dense_timed_robustness_0_setting
}  // namespace reelay
/*
 * Copyright (c) 2019-2020 Dogan Ulus
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
struct atomic_ref final : public dense_timed_state<X, data_interval_map<T>, T> {
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
                     manager->zero()));
                     
  key_t key;
  std::string variable;

  explicit atomic_ref(const data_mgr_t &mgr, const key_t &k,
                       const std::string &c)
      : manager(mgr), key(k), variable(c) {
    manager->add_variable(variable);
  }

  explicit atomic_ref(const kwargs &kw)
      : atomic_ref(reelay::any_cast<data_mgr_t>(kw.at("manager")),
                    reelay::any_cast<key_t>(kw.at("key")),
                    reelay::any_cast<std::string>(kw.at("constant"))) {}

  void update(const input_t &pargs, const input_t &args, time_t previous,
              time_t now) override {

    if (not datafield<input_t>::contains(args, key)) {
      return; // Do nothing if the key does not exist - existing value persists
    }

    std::string new_data = datafield<input_t>::as_string(args, key);

    value = value - interval::left_open(0, previous);
    value = value - interval::left_open(now, infinity<time_t>::value());

    value.add(
        std::make_pair(interval::left_open(now, infinity<time_t>::value()),
                       manager->assign(variable, new_data)));
  }

  output_t output(time_t previous, time_t now) override {
    return (value - interval::left_open(0, previous)) -
           interval::left_open(now, infinity<time_t>::value());
  }
};

} // namespace untimed_setting
} // namespace reelay
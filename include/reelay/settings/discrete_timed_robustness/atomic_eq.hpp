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
namespace discrete_timed_robustness_setting {

template <typename X, typename V, typename T, typename K = std::string>
struct atomic_eq final : public discrete_timed_state<X, V, T> {
  using key_t = K;
  using time_t = T;
  using input_t = X;
  using output_t = V;

  key_t key;
  double constant;

  output_t value = -reelay::infinity<output_t>::value();

  explicit atomic_eq(const key_t &k, const std::string &c_str)
      : key(k), constant(std::stod(c_str)) {}

  explicit atomic_eq(const kwargs &kw)
      : atomic_eq(reelay::any_cast<key_t>(kw.at("key")),
                   reelay::any_cast<std::string>(kw.at("constant"))) {}

  void update(const input_t &args, time_t) override {

    if (not datafield<input_t>::contains(args, key)) {
      return; // Do nothing if the key does not exist - existing value persists
    }

    double new_data = datafield<input_t>::as_floating(args, key);

    value = std::min(constant - new_data, new_data - constant);
  }

  output_t output(time_t) override { return value; }
};

} // namespace untimed_setting
} // namespace reelay
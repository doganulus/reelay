/*
 * Copyright (c) 2019-2023 Dogan Ulus
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
struct atomic_any final : public discrete_timed_state<X, V, T> {
  using key_t = K;
  using time_t = T;
  using input_t = X;
  using output_t = V;

  key_t key;

  output_t value = -reelay::infinity<output_t>::value();

  explicit atomic_any(const key_t &key) : key(key) {}

  explicit atomic_any(const kwargs &kw)
      : atomic_any(reelay::any_cast<key_t>(kw.at("key"))) {}

  void update(const input_t &args, time_t) override {
    bool key_exists = datafield<input_t>::contains(args, key);

    if (key_exists) {
      value = reelay::infinity<output_t>::value();
    } else {
      value = -reelay::infinity<output_t>::value();
    }
  }

  output_t output(time_t) override { return value; }
};

} // namespace untimed_setting
} // namespace reelay
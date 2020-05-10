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
namespace discrete_timed_setting {

template <typename X, typename T, typename K = std::string>
struct atomic_prop final : public discrete_timed_state<X, bool, T> {
  using key_t = K;
  using time_t = T;
  using input_t = X;
  using output_t = bool;

  key_t key;

  bool value = false;

  explicit atomic_prop(const key_t &key) : key(key) {}

  explicit atomic_prop(const kwargs &kw)
      : atomic_prop(reelay::any_cast<key_t>(kw.at("key"))) {}

  void update(const input_t &args, time_t) override {

    if (not datafield<input_t>::contains(args, key)) {
      return; // Do nothing if the key does not exist - existing value persists
    }

    bool new_data = datafield<input_t>::as_bool(args, key);

    value = new_data;
  }

  output_t output(time_t) override { return value; }
};

} // namespace untimed_setting
} // namespace reelay
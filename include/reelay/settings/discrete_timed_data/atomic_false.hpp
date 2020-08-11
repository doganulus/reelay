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
#include "reelay/networks/basic_structure.hpp"
#include "reelay/unordered_data.hpp"

namespace reelay {
namespace discrete_timed_data_setting {

template <typename X, typename T, typename K = std::string>
struct atomic_false final : public discrete_timed_state<X, data_set_t, T> {
  using key_t = K;
  using time_t = T;
  using input_t = X;
  using value_t = data_set_t;
  using output_t = data_set_t;

  data_mgr_t manager;
  data_set_t value;

  key_t key;

  explicit atomic_false(const data_mgr_t &mgr, const key_t &key)
      : manager(mgr), value(mgr->zero()), key(key) {}

  explicit atomic_false(const kwargs &kw)
      : atomic_false(reelay::any_cast<data_mgr_t>(kw.at("manager")),
                      reelay::any_cast<key_t>(kw.at("key"))) {}

  void update(const input_t &args, time_t now) override {

    if (not datafield<input_t>::contains(args, key)) {
      return; // Do nothing if the key does not exist - existing value persists
    }

    bool new_data = datafield<input_t>::as_bool(args, key);

    if (new_data) {
      value = manager->zero();
    } else {
      value = manager->one();
    }
  }

  output_t output(time_t) override { return value; }
};

} // namespace untimed_setting
} // namespace reelay
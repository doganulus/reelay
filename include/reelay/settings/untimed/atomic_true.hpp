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
namespace untimed_setting {

template <typename X, typename K = std::string>
struct atomic_true : public untimed_state<X, bool> {
  using key_t = K;
  using input_t = X;
  using output_t = bool;

  using node_t = untimed_node<output_t>;
  using node_ptr_t = std::shared_ptr<node_t>;

  key_t key;

  bool value = false;

  explicit atomic_true(const key_t &key) : key(key) {}

  explicit atomic_true(const kwargs &kw)
      : atomic_true(reelay::any_cast<key_t>(kw.at("key"))) {}

  void update(const input_t &args) override {

    bool new_data;

    try {
      new_data = datafield<input_t>::as_bool(args, key);
    } catch (const std::out_of_range &e) {
      return; // Do nothing if the key does not exist - existing value persists
    }

    value = new_data;
  }

  output_t output() override { return value; }
};

} // namespace untimed_setting
} // namespace reelay
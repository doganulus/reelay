/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "string"

#include "reelay/common.hpp"
#include "reelay/networks/basic_structure.hpp"

namespace reelay {
namespace untimed_setting {

template <typename X>
struct predicate final : public untimed_state<X, bool> {
  using input_t = X;
  using output_t = bool;

  using function_t = std::function<bool(const input_t &)>;

  std::function<bool(const input_t &)> fn;
  bool value = false;

  explicit predicate(const function_t &f) : fn(f) {}

  explicit predicate(const kwargs &kw)
      : predicate(reelay::any_cast<function_t>(kw.at("function"))) {}

  void update(const input_t &args) override { value = fn(args); }

  output_t output() override { return value; }
};

} // namespace untimed_setting
} // namespace reelay
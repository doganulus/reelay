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
namespace untimed_robustness_setting {

template <typename X, typename V>
struct predicate final : public untimed_state<X, V> {
  using input_t = X;
  using output_t = V;

  using function_t = std::function<output_t(const input_t &)>;

  std::function<output_t(const input_t &)> fn;
  output_t value = -reelay::infinity<output_t>::value();

  explicit predicate(const function_t &f) : fn(f) {}

  explicit predicate(const kwargs &kw)
      : predicate(reelay::any_cast<function_t>(kw.at("function"))) {}

  void update(const input_t &args) override { value = fn(args); }

  output_t output() override { return value; }
};

} // namespace untimed_robustness_setting
} // namespace reelay
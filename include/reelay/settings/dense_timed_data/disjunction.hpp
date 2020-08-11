/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>
#include <vector>

#include "reelay/common.hpp"
#include "reelay/intervals.hpp"
#include "reelay/networks/basic_structure.hpp"
#include "reelay/unordered_data.hpp"

namespace reelay {
namespace dense_timed_data_setting {

template <typename X, typename T>
struct disjunction final : public dense_timed_node<data_interval_map<T>, T> {
  using time_t = T;
  using input_t = X;
  using value_t = data_set_t;
  using output_t = reelay::data_interval_map<time_t>;

  using node_t = dense_timed_node<output_t, time_t>;
  using state_t = dense_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  using interval = reelay::interval<time_t>;
  using interval_map = reelay::data_interval_map<time_t>;

  std::vector<node_ptr_t> args;

  explicit disjunction(const std::vector<node_ptr_t> &args) : args(args) {}

  explicit disjunction(const kwargs &kw)
      : disjunction(reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}

  output_t output(time_t previous, time_t now) override {
    auto result = args[0]->output(previous, now);
    for (size_t i = 1; i < args.size(); i++) {
      // Problem: Boost ICL interval_map calls identity element
      // when operator+ used. We cannot define a static identity element
      // for BDDs as BDD true/false depends on managers constructed at runtime.
      //
      // Ideal: result += args[i]->output(previous, now);
      //
      // Workaround: Adding does not call the identity but apply the operation.
      for (const auto &intv : args[i]->output(previous, now)) {
        result.add(intv);
      }
    }

    return result;
  }
};

} // namespace dense_timed_data_setting
} // namespace reelay
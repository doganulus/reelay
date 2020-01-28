/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "vector"

#include "reelay/intervals.hpp"
#include "reelay/networks/basic_structure.hpp"

namespace reelay {
namespace dense_timed_setting {

template <typename X, typename T>
struct conjunction : public dense_timed_node<interval_set<T>, T> {
  using time_t = T;
  using input_t = X;
  using output_t = reelay::interval_set<time_t>;

  using node_t = dense_timed_node<output_t, time_t>;
  using node_ptr_t = std::shared_ptr<node_t>;

  using interval = reelay::interval<time_t>;
  using interval_set = reelay::interval_set<time_t>;

  std::vector<node_ptr_t> args;

  explicit conjunction(const std::vector<node_ptr_t> &args) : args(args) {}

  explicit conjunction(const kwargs &kw)
      : conjunction(reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}

  output_t output(time_t previous, time_t now) {
    interval_set result = args[0]->output(previous, now);
    for (size_t i = 1; i < args.size(); i++) {
      result = result & args[i]->output(previous, now);
    }
    return result;
  }
};

}  // namespace dense_timed_setting
}  // namespace reelay
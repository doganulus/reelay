/*
 * Copyright (c) 2019-2023 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <vector>

#include "reelay/common.hpp"
#include "reelay/intervals.hpp"
#include "reelay/networks/basic_structure.hpp"
#include "reelay/unordered_data.hpp"

namespace reelay {
namespace discrete_timed_data_setting {

template <typename X, typename T>
struct past_always_bounded final
    : public discrete_timed_state<X, data_set_t, T> {
  using time_t = T;
  using input_t = X;
  using value_t = data_set_t;
  using output_t = data_set_t;

  using node_t = discrete_timed_node<output_t, time_t>;
  using state_t = discrete_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  using interval = reelay::interval<time_t>;
  using interval_map = reelay::data_interval_map<time_t>;

  data_mgr_t manager;

  interval_map value = interval_map();

  node_ptr_t first;

  time_t lbound;
  time_t ubound;

  past_always_bounded(const data_mgr_t &mgr,
                      const std::vector<node_ptr_t> &args, time_t l = 0,
                      time_t u = 0)
      : manager(mgr), first(args[0]), lbound(l), ubound(u) {
    value.add(std::make_pair(
        interval::closed(-reelay::infinity<time_t>::value(), lbound),
        mgr->one()));
  }

  explicit past_always_bounded(const kwargs &kw)
      : past_always_bounded(
            reelay::any_cast<data_mgr_t>(kw.at("manager")),
            reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args")),
            reelay::any_cast<time_t>(kw.at("lbound")),
            reelay::any_cast<time_t>(kw.at("ubound"))) {}

  void update(const input_t &, time_t now) {
    value.add(std::make_pair(
        reelay::interval<time_t>::closed(now + lbound, now + ubound),
        ~first->output(now)));
    value -= interval_map(std::make_pair(
        reelay::interval<time_t>::right_open(0, now), manager->zero()));
  }

  output_t output(time_t now) { return ~value(now); }
};

} // namespace discrete_timed_data_setting
} // namespace reelay
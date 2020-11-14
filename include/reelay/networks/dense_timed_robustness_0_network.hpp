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
//
#include "reelay/datafield.hpp"
#include "reelay/intervals.hpp"
#include "reelay/parser/ptl.hpp"
//
#include "reelay/networks/basic_structure.hpp"
#include "reelay/settings/dense_timed_robustness_0/setting.hpp"
//
#include "reelay/options.hpp"

namespace reelay {

template <typename T, typename V, typename X>
struct dense_timed_robustness_0_network
    : public dense_timed_state<X, robustness_interval_map<T, V>, T> {
  using time_t = T;
  using value_t = V;
  using base_t = reelay::robustness_interval_map<time_t, value_t>;
  using input_t = X;
  using output_t = base_t;

  using type = dense_timed_robustness_0_network<time_t, value_t, input_t>;

  using interval = reelay::interval<time_t>;
  using interval_map = reelay::robustness_interval_map<time_t, value_t>;

  using node_t = dense_timed_node<base_t, time_t>;
  using state_t = dense_timed_state<input_t, base_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  using setting_t = dense_timed_robustness_0_setting::factory<input_t, value_t, time_t>;
  using options_t = basic_options;

  node_ptr_t root;
  std::vector<state_ptr_t> states;

  input_t prevargs = input_t{};

  time_t previous = 0;  // Time Zero
  time_t current = 0;   // Time Zero

  time_t now() const { return current; }

  dense_timed_robustness_0_network(
      const node_ptr_t &n, const std::vector<state_ptr_t> &ss)
      : root(n), states(ss) {}

  dense_timed_robustness_0_network(
      const node_ptr_t &n, const std::vector<state_ptr_t> &ss,
      const options_t &options)
      : dense_timed_robustness_0_network(n, ss) {}

  void update(
      const input_t &pargs, const input_t &args, time_t previous,
      time_t now) override {
    for (const auto &state : this->states) {
      state->update(pargs, args, previous, now);
    }
  }

  base_t output(time_t previous, time_t now) override {
    return root->output(previous, now);
  }

  base_t update(const input_t &args) {
    previous = current;
    current = timefield<time_t, input_t>::get_time(args);

    this->update(prevargs, args, previous, current);

    prevargs = args;

    return output(previous, current);
  }

  base_t output() {
    return output(previous, current);
  }

  static type make(
      const std::string &pattern, const options_t &options = options_t()) {
    auto parser = ptl_parser<type>();
    return parser.parse(pattern, options);
  }
  
  static std::shared_ptr<type> make_shared(
      const std::string &pattern, const options_t &options = options_t()) {
    auto parser = ptl_parser<type>();
    return parser.make_shared(pattern, options);
  }
};

}  // namespace reelay

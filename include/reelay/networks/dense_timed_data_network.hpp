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
#include "reelay/networks/basic_structure.hpp"
#include "reelay/parser/ptl.hpp"
#include "reelay/settings/dense_timed_data/setting.hpp"
#include "reelay/unordered_data.hpp"
//
#include "reelay/options.hpp"

namespace reelay {

template <typename T, typename X>
struct dense_timed_data_network
    : public dense_timed_state<X, data_interval_map<T>, T> {
  using time_t = T;
  using value_t = data_set_t;
  using base_t = reelay::data_interval_map<time_t>;
  using input_t = X;
  using output_t = base_t;

  using type = dense_timed_data_network<time_t, input_t>;

  using node_t = dense_timed_node<output_t, time_t>;
  using state_t = dense_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  using interval = reelay::interval<time_t>;
  using interval_map = reelay::data_interval_map<time_t>;

  using setting_t = dense_timed_data_setting::factory<input_t, time_t>;
  using options_t = basic_options;

  data_mgr_t manager;
  node_ptr_t root;
  std::vector<state_ptr_t> states;

  input_t prevargs = input_t{};

  time_t previous = 0;  // Time Zero
  time_t current = 0;   // Time Zero

  dense_timed_data_network(
      const data_mgr_t mgr, const node_ptr_t &n,
      const std::vector<state_ptr_t> &ss)
      : manager(mgr), root(n), states(ss) {}

  dense_timed_data_network(
      const node_ptr_t &n, const std::vector<state_ptr_t> &ss,
      const options_t &options)
      : dense_timed_data_network(options.get_data_manager(), n, ss) {}

  void update(const input_t &pargs, const input_t &args, time_t tp, time_t tn)
      override {
    for (const auto &state : this->states) {
      state->update(pargs, args, tp, tn);
    }
  }

  time_t now() const { return current; }

  output_t output(time_t tp, time_t tn) override {
    return root->output(tp, tn);
  }

  output_t update(const input_t &args) {
    previous = current;
    current = timefield<time_t, input_t>::get_time(args);

    this->update(prevargs, args, previous, current);

    prevargs = args;

    return this->output(previous, current);
  }

  output_t output() {
    return output(previous, current);
  }

  static type make(
      const std::string &pattern, const options_t &options = options_t()) {
    //
    // Workaround until new parser
    auto manager = options.get_data_manager();
    kwargs kw = {{"manager", manager}};

    auto parser = ptl_parser<type>(kw);
    return parser.parse(pattern, options);
  }
  
  static std::shared_ptr<type> make_shared(
      const std::string &pattern, const options_t &options = options_t()) {
    //
    // Workaround until new parser
    auto manager = options.get_data_manager();
    kwargs kw = {{"manager", manager}};

    auto parser = ptl_parser<type>(kw);
    return parser.make_shared(pattern, options);
  }
};

}  // namespace reelay

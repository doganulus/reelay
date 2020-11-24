#ifndef REELAY_MONITORS_DISCRETE_TIMED_DATA_MONITOR_HPP
#define REELAY_MONITORS_DISCRETE_TIMED_DATA_MONITOR_HPP

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
#include <utility>

#include "reelay/formatters/formatter.hpp"
#include "reelay/monitors/abstract_monitor.hpp"
#include "reelay/networks/discrete_timed_data_network.hpp"
//
#include "reelay/options.hpp"

namespace reelay {

template <
    typename TimeT, typename InputT, typename OutputT,
    bool condensing = true>
struct discrete_timed_data_monitor final
    : public abstract_monitor<InputT, OutputT> {
  using time_type = TimeT;
  using value_type = bool;
  using input_type = InputT;
  using output_type = OutputT;

  using type = discrete_timed_data_monitor<
      TimeT, InputT, OutputT, condensing>;

  using base_output_type = data_set_t;

  using node_type = discrete_timed_node<base_output_type, time_type>;
  using state_type
      = discrete_timed_state<input_type, base_output_type, time_type>;

  using network_t = discrete_timed_data_network<time_type, input_type>;
  using formatter_t
      = discrete_timed_formatter<time_type, value_type, output_type, condensing>;

  discrete_timed_data_monitor() = default;

  explicit discrete_timed_data_monitor(
      data_mgr_t  mgr, const network_t &n, const formatter_t &f)
      : manager(std::move(mgr)), network(n), formatter(f) {}

  output_type now() override {
    return formatter.now(network.now());
  }

  output_type update(const input_type &args) override {
    auto result = network.update(args);
    return formatter.format(result != manager->zero(), network.now());
  }
  
  static type make(const std::string &pattern, const basic_options &options) {
    auto mgr = options.get_data_manager();
    auto net = network_t::make(pattern, options);
    auto fmt = formatter_t(options);
    return type(mgr, net, fmt);
  }

  static std::shared_ptr<type> make_shared(
      const std::string &pattern, const basic_options &options) {
    auto mgr = options.get_data_manager();
    auto net = network_t::make(pattern, options);
    auto fmt = formatter_t(options);
    return std::make_shared<type>(mgr, net, fmt);
  }

 private:
  data_mgr_t manager;
  network_t network;
  formatter_t formatter;
};
}  // namespace reelay
#endif

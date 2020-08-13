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

#include "reelay/formatters/formatter.hpp"
#include "reelay/monitors/abstract_monitor.hpp"
#include "reelay/networks/discrete_timed_robustness_network.hpp"
//

//
#include "reelay/options.hpp"

namespace reelay {

template <
    typename TimeT, typename ValueT, typename InputT, typename OutputT,
    bool condensing = true>
struct discrete_timed_robustness_monitor final
    : public abstract_monitor<InputT, OutputT> {
  using time_type = TimeT;
  using value_type = ValueT;
  using input_type = InputT;
  using output_type = OutputT;

  using type = discrete_timed_robustness_monitor<
      time_type, value_type, input_type, output_type, condensing>;

  using network_t
      = discrete_timed_robustness_network<time_type, value_type, input_type>;
  using formatter_t = discrete_timed_formatter<
      time_type, value_type, output_type, condensing>;

  discrete_timed_robustness_monitor() = default;

  explicit discrete_timed_robustness_monitor(
      const network_t &n, const formatter_t &f)
      : network(n), formatter(f) {}

  output_type now() override {
    return formatter.now(network.now);
  }

  output_type update(const input_type &args) override {
    auto result = network.update(args);
    return formatter.format(result, network.now);
  }
  
  static type make(const std::string &pattern, const basic_options &options) {
    auto net = network_t::make(pattern, options);
    auto formatter = formatter_t(options);
    return type(net, formatter);
  }

  static std::shared_ptr<type> make_shared(
      const std::string &pattern, const basic_options &options) {
    auto net = network_t::make(pattern, options);
    auto formatter = formatter_t(options);
    return std::make_shared<type>(net, formatter);
  }

 private:
  network_t network;
  formatter_t formatter;
};
}  // namespace reelay
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
#include "reelay/networks/dense_timed_network.hpp"
//
#include "reelay/options.hpp"

namespace reelay {

template <typename TimeT, typename InputT, typename OutputT>
struct dense_timed_monitor final : public abstract_monitor<InputT, OutputT> {
  using time_type = TimeT;
  using value_type = bool;
  using input_type = InputT;
  using output_type = OutputT;

  using type = dense_timed_monitor<time_type, input_type, output_type>;

  using network_t = dense_timed_network<time_type, input_type>;
  using formatter_t = dense_timed_formatter<time_type, value_type, output_type>;

  dense_timed_monitor() = default;

  explicit dense_timed_monitor(const network_t &n, const formatter_t &f)
      : network(n), formatter(f) {}

  output_type now() override {
    return formatter.now(network.current);
  }

  output_type update(const input_type &args) override {
    auto result = network.update(args);
    return formatter.format(result, network.previous, network.current);
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
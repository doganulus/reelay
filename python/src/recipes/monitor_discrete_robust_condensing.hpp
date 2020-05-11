/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "reelay/parser/ptl.hpp"
#include "reelay/pybind11.hpp"
#include "reelay/settings.hpp"
#include "string"

#include "reelay/targets/pybind11/condensing_python_formatter.hpp"

namespace reelay {

struct monitor_discrete_robust_condensing {
  using time_t = int64_t;
  using value_t = double;
  using input_t = pybind11::object;
  using output_t = pybind11::dict;

  using factory
      = discrete_timed_robustness_setting::factory<input_t, value_t, time_t>;

  using network_t = typename factory::network_t;
  using network_ptr_t = typename factory::network_ptr_t;

  using formatter_t = condensing_python_formatter<time_t, value_t>;

  network_ptr_t network;
  formatter_t formatter;

  explicit monitor_discrete_robust_condensing(
      const std::string &pattern, const std::string &t_str = "time",
      const std::string &y_str = "value")
      : formatter(formatter_t(t_str, y_str)) {
    auto parser = ptl_parser<factory>();
    this->network = parser.parse(pattern);
  }

  output_t update(const input_t &args) {
    this->network->update(args);
    return formatter.format(network->output(), network->now());
  }

  time_t now() { return network->now(); }
};

}  // namespace reelay

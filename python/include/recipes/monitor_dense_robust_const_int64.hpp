/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "string"

#include "reelay/parser/ptl.hpp"
#include "reelay/settings.hpp"
#include "reelay/pybind11.hpp"


#include "reelay/targets/pybind11/dense_timed_robustness_python_formatter.hpp"

namespace reelay {

struct monitor_dense_robust_const_int64 {

  using time_t = int64_t;
  using value_t = double;
  using input_t = pybind11::object;
  using output_t = pybind11::list;

  using factory =
      dense_timed_robustness_0_setting::factory<input_t, value_t, time_t>;

  using network_t = typename factory::network_t;
  using network_ptr_t = typename factory::network_ptr_t;

  using formatter_t = dense_timed_robustness_python_formatter<time_t, value_t>;
  
  network_ptr_t network;
  formatter_t formatter;

  value_t lastval = 0;

  explicit monitor_dense_robust_const_int64(
      const std::string &pattern, const std::string t_str = "time",
      const std::string y_str = "value")
      : formatter(formatter_t(t_str, y_str)) {
    auto parser = ptl_parser<factory>();
    this->network = parser.parse(pattern);
  }

  output_t update(const input_t &args) {
    this->network->update(args);
    auto result = network->output();
    return formatter.format(result, network->previous, network->current);
  }

  time_t now() { return network->now(); }
};

} // namespace reelay

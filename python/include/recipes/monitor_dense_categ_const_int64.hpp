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

#include "reelay/targets/pybind11/dense_timed_data_python_formatter.hpp"

namespace reelay {

struct monitor_dense_categ_const_int64 {

  using time_t = int64_t;
  using input_t = pybind11::object;
  using output_t = pybind11::list;

  using factory = dense_timed_data_setting::factory<input_t, time_t>;

  using network_t = typename factory::network_t;
  using network_ptr_t = typename factory::network_ptr_t;

  using formatter_t = dense_timed_data_python_formatter<time_t>;

  data_mgr_t manager;
  network_ptr_t network;
  formatter_t formatter;

  bool lastval;

  explicit monitor_dense_categ_const_int64(
      const std::string &pattern, const std::string t_str = "time",
      const std::string y_str = "value")
      : formatter(formatter_t(t_str, y_str)) {
    manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs kw = {{"manager", manager}};

    auto parser = ptl_parser<factory>(kw);
    this->network = parser.parse(pattern);
  }

  output_t update(const input_t &args) {
    this->network->update(args);
    auto result = network->output();
    return formatter.format(
        manager, result, network->previous, network->current);
  }

  time_t now() { return network->now(); }
};

} // namespace reelay

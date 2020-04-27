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

namespace reelay {

struct monitor_dense_categ_const_float64 {

  using time_t = double;
  using input_t = pybind11::object;
  using output_t = pybind11::list;

  using factory = dense_timed_data_setting::factory<input_t, time_t>;

  using network_t = typename factory::network_t;
  using network_ptr_t = typename factory::network_ptr_t;

  const std::string t_name;
  const std::string y_name;

  data_mgr_t manager;
  network_ptr_t network;

  bool lastval;

  explicit monitor_dense_categ_const_float64(const std::string &pattern,
                                             const std::string t_str = "time",
                                             const std::string y_str = "value")
      : t_name(t_str), y_name(y_str) {

    manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs kw = {{"manager", manager}};

    auto parser = ptl_parser<factory>(kw);
    this->network = parser.parse(pattern);
  }

  output_t update(const input_t &args) {
    this->network->update(args);

    output_t vresult;
    for (const auto &intv : this->network->output()) {
      bool value = (intv.second == manager->one());
      if (lastval != value or network->current == 0) {
        vresult.append(pybind11::dict(
            pybind11::arg(t_name.c_str()) = intv.first.lower(),
            pybind11::arg(y_name.c_str()) = (intv.second == manager->one())));
        lastval = value;
      }
    }
    return vresult;
  }

  std::pair<time_t, time_t> now() { return network->now(); }
};

} // namespace reelay

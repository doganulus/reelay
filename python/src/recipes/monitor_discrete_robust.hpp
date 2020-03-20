/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "reelay/parser/ptl.hpp"
#include "reelay/pydict.hpp"
#include "reelay/settings.hpp"
#include "string"

namespace reelay {

struct monitor_discrete_robust {

  using time_t = int64_t;
  using value_t = double;
  using input_t = pybind11::dict;
  using output_t = pybind11::dict;

  using factory =
      discrete_timed_robustness_setting::factory<input_t, value_t, time_t>;

  using network_t = typename factory::network_t;
  using network_ptr_t = typename factory::network_ptr_t;

  std::string name;

  network_ptr_t network;

  explicit monitor_discrete_robust(const std::string &pattern,
                           const std::string &output_name = "value")
      : name(output_name) {

    auto parser = ptl_parser<factory>();
    this->network = parser.parse(pattern);
  }

  output_t update(const input_t &args) {
    this->network->update(args);
    return pybind11::dict(pybind11::arg(name.c_str()) =
                              this->network->output());
  }

  time_t now() { return network->now(); }
};

} // namespace reelay

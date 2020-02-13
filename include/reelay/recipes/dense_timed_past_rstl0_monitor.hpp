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

namespace reelay {

struct dense_timed_past_rstl0_monitor {

  using time_t = double;
  using value_t = double;
  using input_t = std::unordered_map<std::string, time_t>;
  using output_t = std::vector<std::pair<time_t, value_t>>;
  using factory = dense_timed_robustness_0_setting::factory<input_t, value_t, time_t>;

  using network_t = typename factory::network_t;
  using network_ptr_t = typename factory::network_ptr_t;

  network_ptr_t network;

  explicit dense_timed_past_rstl0_monitor(const std::string &pattern) {
    auto parser = ptl_parser<factory>();
    this->network = parser.parse(pattern);
  }

  output_t update(const input_t &args) {
    this->network->update(args);
    return this->network->moutput();
  }

  std::pair<time_t, time_t> now() { return network->now(); }
};

} // namespace reelay

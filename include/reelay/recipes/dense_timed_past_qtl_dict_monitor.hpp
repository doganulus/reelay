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
#include "reelay/json.hpp"

namespace reelay {

struct dense_timed_past_qtl_dict_monitor {

  using time_t = int64_t;
  using input_t = reelay::json;
  using output_t = std::vector<std::pair<time_t, bool>>;

  using factory = dense_timed_data_setting::factory<input_t, time_t, 0>;

  using network_t = typename factory::network_t;
  using network_ptr_t = typename factory::network_ptr_t;
  
  data_mgr_t manager;
  network_ptr_t network;

  explicit dense_timed_past_qtl_dict_monitor(const std::string &pattern) {

    manager = std::make_shared<reelay::binding_manager>();
    reelay::kwargs kw = {{"manager", manager}};

    auto parser = ptl_parser<factory>(kw);
    this->network = parser.parse(pattern);
  }

  output_t update(const input_t &args) {
    this->network->update(args);
    output_t current_output;
    for (const auto &intv : this->network->output()) {
      current_output.push_back(
          std::make_pair(intv.first.upper(), (intv.second == manager->one())));
    }
    return current_output;
  }

  std::pair<time_t, time_t> now() { return network->now(); }
};

} // namespace reelay
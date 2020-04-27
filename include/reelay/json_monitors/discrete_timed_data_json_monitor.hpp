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

#include "reelay/common.hpp"
#include "reelay/json.hpp"
#include "reelay/networks.hpp"
#include "reelay/parser/ptl.hpp"
#include "reelay/settings.hpp"

namespace reelay {

template <typename T>
struct discrete_timed_data_json_monitor : base_json_monitor<T> {
  using time_t = T;
  using input_t = json;
  using output_t = json;

  using factory = discrete_timed_data_setting::factory<input_t, time_t>;

  using network_t = typename factory::network_t;
  using network_ptr_t = typename factory::network_ptr_t;

  std::string name;

  data_mgr_t manager;
  network_ptr_t network;

  explicit discrete_timed_data_json_monitor(
      const std::string &pattern, const reelay::kwargs &kw = reelay::kwargs()) {
    if (kw.find("manager") == kw.end()) {
      manager = std::make_shared<reelay::binding_manager>();
      reelay::kwargs kw = {{"manager", manager}};
    } else {
      manager = reelay::any_cast<data_mgr_t>(kw.at("manager"));
    }

    auto parser = ptl_parser<factory>(kw);
    this->network = parser.parse(pattern);

    try {
      name = reelay::any_cast<std::string>(kw.at("name"));
    } catch (const std::out_of_range &oor) {
      name = "value";
    }
  }

  output_t update(const input_t &args) {
    this->network->update(args);
    reelay::json j = {{name, this->network->output() == manager->one()}};
    return j;
    // return json::object({name, this->network->output()});
  }

  output_t update(const input_t &args, time_t now) {
    this->network->update(args, now);
    reelay::json j = {{name, this->network->output() == manager->one()}};
    return j;
    // return json::object({name, this->network->output()});
  }

  time_t now() { return network->now(); }
};
}  // namespace reelay
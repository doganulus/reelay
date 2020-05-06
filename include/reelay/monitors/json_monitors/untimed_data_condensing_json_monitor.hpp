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

#include "reelay/targets/json/condensing_json_formatter.hpp"

namespace reelay {

template <typename TimeT,
          class FormatterT = condensing_json_formatter<TimeT>>
struct untimed_data_condensing_json_monitor {
  using time_t = TimeT;
  using input_t = json;
  using output_t = json;

  using factory = untimed_data_setting::factory<input_t>;

  using network_t = typename factory::network_t;
  using network_ptr_t = typename factory::network_ptr_t;

  using formatter_t = FormatterT;

  data_mgr_t manager;
  network_ptr_t network;
  formatter_t formatter;

  std::string t_name = "time";
  std::string y_name = "value";

  untimed_data_condensing_json_monitor() = default; 

  explicit untimed_data_condensing_json_monitor(
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
      this->y_name = reelay::any_cast<std::string>(kw.at("name"));
    } catch (const std::out_of_range &oor) {
      this->y_name = "value";
    }
  }

  output_t update(const input_t &args) {
    this->network->update(args);
    return formatter.format((network->output() == manager->one()),
                             network->now());
  }

  time_t now() { return this->network->now(); }
};
}  // namespace reelay
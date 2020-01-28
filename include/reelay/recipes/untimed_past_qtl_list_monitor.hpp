/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "string"
#include "reelay/parser/ptl_parser.hpp"
#include "reelay/settings.hpp"

namespace reelay {

struct untimed_past_qtl_list_monitor{

  using input_t = std::vector<std::string>;
  using factory = untimed_data_setting::factory<input_t>;

  using network_t = typename factory::network_t;
  using network_ptr_t = typename factory::network_ptr_t;

  data_mgr_t manager;
  network_ptr_t network;

  explicit untimed_past_qtl_list_monitor(const std::string &pattern) {

    manager = std::make_unique<reelay::binding_manager>();
    reelay::kwargs kw = {{"manager", manager}};

    auto parser = ptl_parser<factory>(kw);
    this->network = parser.parse(pattern);
  }

  bool update(const input_t &args) {
    this->network->update(args);
    return this->network->output() == manager->one();
  }
  
  int64_t now() { return network->now(); }
};

} // namespace reelay
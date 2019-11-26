/*
 * Copyright (c) 2019 Dogan Ulus
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

struct past_ltl_monitor{

  using input_t = std::map<std::string, bool>;
  using factory = untimed_setting::factory<input_t>;

  using network_t = typename factory::network_t;
  using network_ptr_t = typename factory::network_ptr_t;

  network_ptr_t network;

  past_ltl_monitor(const std::string &pattern) {
    auto parser = ptl_parser<factory>();
    this->network = parser.parse(pattern);
  }

  void update(const input_t &args) { this->network->update(args); }
  bool output() { return this->network->output(); }
};

} // namespace reelay

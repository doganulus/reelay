/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "reelay/settings.hpp"
#include "reelay/targets/stdout/untimed/stdout_formatter.hpp"

namespace reelay {
namespace untimed_setting {

template <typename X>
struct stdout_formatter_verbosity_0 : public stdout_formatter<X> {
  using input_t = X;
  using output_t = std::string;

  using network_t = untimed_network<input_t, bool>;
  using network_ptr_t = std::shared_ptr<network_t>;

  using strings_t = std::vector<std::string>;

  network_ptr_t network;
  strings_t columm_names;

  stdout_formatter_verbosity_0(network_ptr_t netptr, const strings_t &names)
      : network(netptr), columm_names(names) {}

  void update(const input_t &args) { network->update(args); }

  std::string header() { return std::string(); }
  std::string output() {
    std::ostringstream buffer;
    if (not network->output()){
      buffer << "False at " << network->now << std::endl;
    }
    return buffer.str();
  }
}; // namespace discrete_timed_setting

} // namespace reelay
} // namespace reelay
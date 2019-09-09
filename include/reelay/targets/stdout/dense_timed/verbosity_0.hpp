/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "memory"

#include "reelay/settings.hpp"
#include "reelay/targets/stdout/dense_timed/formatter.hpp"

namespace reelay {
namespace dense_timed_setting {

template <typename X, typename T>
struct stdout_formatter_verbosity_0 : stdout_formatter<X, T> {
  using factory = dense_timed_setting::factory<X, T>;

  using input_t = typename factory::input_t;
  using output_t = typename factory::output_t;
  using function_t = typename factory::function_t;

  using network_t = typename factory::network_t;
  using network_ptr_t = typename factory::network_ptr_t;

  stdout_formatter_verbosity_0(network_ptr_t network) : child(network) {}

  std::string header(const std::vector<std::string>& columm_names) override {
    return std::string();
  }
  std::string output() override {
    output_t result =
        this->child->output(network->previous, network->current);

    std::ostringstream buffer;
    for (const auto& intv : result) {
      buffer << "Fails between times " << intv.lower() << " and "
             << intv.upper() << std::endl;
    }

    return buffer.str();
  }
};

}  // namespace dense_timed_setting
}  // namespace reelay
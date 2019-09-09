/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "reelay/settings.hpp"
#include "reelay/targets/stdout/dense_timed/formatter.hpp"

namespace reelay {
namespace dense_timed_setting {

template <typename X, typename T>
struct stdout_formatter_verbosity_1 : public stdout_formatter<X, T> {
  using factory = dense_timed_setting::factory<X, T>;

  using input_t = typename factory::input_t;
  using output_t = typename factory::output_t;
  using function_t = typename factory::function_t;

  using network_t = typename factory::network_t;
  using network_ptr_t = typename factory::network_ptr_t;

  std::string get_header(
      const std::vector<std::string>& columm_names) override {
    std::ostringstream buffer;

    buffer << "time"
           << ",";
    buffer << "out" << std::endl;

    return buffer.str();
  }
  std::string get_output(const network_t& network) override {
    output_t result =
        network->output_node->output(network->previous, network->current);
    std::ostringstream buffer;

    time_t t;

    if (result.empty()) {
      buffer << network->current << ","
             << "0" << std::endl;
    }

    for (const auto& intv : result) {
      if (intv.lower() > network->previous) {
        t = intv.upper();
        buffer << intv.lower() << ","
               << "0" << std::endl;
        buffer << intv.upper() << ","
               << "1" << std::endl;
      } else if (intv.upper() < network->current) {
        t = intv.upper();
        buffer << intv.upper() << ","
               << "1" << std::endl;
      }
    }

    if (t < network->current) {
      buffer << network->current << ","
             << "0" << std::endl;
    }

    return buffer.str();
  }
};

}  // namespace dense_timed_setting
}  // namespace reelay
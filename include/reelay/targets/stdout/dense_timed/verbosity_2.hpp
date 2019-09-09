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
struct stdout_formatter_verbosity_2 : stdout_formatter<X, T> {
  using factory = dense_timed_setting::factory<X, T>;

  using input_t = typename factory::input_t;
  using output_t = typename factory::output_t;
  using function_t = typename factory::function_t;

  using network_t = typename factory::network_t;
  using network_ptr_t = typename factory::network_ptr_t;

  stdout_formatter_verbosity_2(network_ptr_t network,
                               std::vector<std::string> columns) {
    this->network = network;
    this->columms = columns;
  }

  std::string header() override {
    std::ostringstream buffer;

    buffer << "time"
           << ",";
    for (auto const& column_name : this->columms) {
      if (column_name != "time") {
        buffer << column_name << ",";
      }
    }
    buffer << "out" << std::endl;

    return buffer.str();
  }

  std::string output() override {
    output_t result = this->network->output(
        this->network->previous, this->network->current);
    std::ostringstream buffer;

    time_t t;

    if (result.empty()) {
      buffer << this->network->current << ",";
      for (auto const& [k, v] : this->network->prevargs) {
        if (k != "time") {
          buffer << v << ",";
        }
      }
      buffer << "0" << std::endl;
    }

    for (const auto& intv : result) {
      if (intv.lower() > this->network->previous) {
        t = intv.upper();
        buffer << intv.lower() << ",";
        for (auto const& [k, v] : this->network->prevargs) {
          if (k != "time") {
            buffer << v << ",";
          }
        }
        buffer << "0" << std::endl;

        buffer << intv.upper() << ",";
        for (auto const& [k, v] : this->network->prevargs) {
          if (k != "time") {
            buffer << v << ",";
          }
        }
        buffer << "1" << std::endl;

      } else {
        t = intv.upper();
        buffer << intv.upper() << ",";
        for (auto const& [k, v] : this->network->prevargs) {
          if (k != "time") {
            buffer << v << ",";
          }
        }
        buffer << "1" << std::endl;
      }
    }

    if (t < this->network->current) {
      buffer << this->network->current << ",";
      for (auto const& [k, v] : this->network->prevargs) {
        if (k != "time") {
          buffer << v << ",";
        }
      }
      buffer << "0" << std::endl;
    }

    return buffer.str();
  }
};

}  // namespace dense_timed_setting
}  // namespace reelay
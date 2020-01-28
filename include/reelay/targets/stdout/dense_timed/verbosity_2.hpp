/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "reelay/intervals.hpp"
#include "reelay/settings.hpp"
#include "reelay/targets/stdout/dense_timed/stdout_formatter.hpp"

namespace reelay {
namespace dense_timed_setting {

template <typename X, typename T>
struct stdout_formatter_verbosity_2 : public stdout_formatter<X, T> {
  using time_t = T;
  using value_t = bool;
  using input_t = X;
  using output_t = std::string;

  using interval_t = reelay::interval<time_t>;
  using interval_set_t = reelay::interval_set<time_t>;

  using network_t =
      dense_timed_network<input_t, interval_set_t, time_t, value_t>;
  using network_ptr_t = std::shared_ptr<network_t>;

  using strings_t = std::vector<std::string>;

  network_ptr_t network;
  strings_t columm_names;

  stdout_formatter_verbosity_2(network_ptr_t netptr, const strings_t &names)
      : network(netptr), columm_names(names) {}

  void update(const input_t &args) { network->update(args); }
  void update(const input_t &args, time_t now) { network->update(args, now); }
  void update(const input_t &pargs, const input_t &args, time_t previous,
              time_t now) override {
    network->update(pargs, args, previous, now);
  }

  std::string header() {
    std::ostringstream buffer;

    buffer << "time"
           << ",";
    for (auto const &column_name : columm_names) {
      if (column_name != "time") {
        buffer << column_name << ",";
      }
    }
    buffer << "out" << std::endl;

    return buffer.str();
  }

  std::string output() {
    time_t t;
    std::ostringstream buffer;

    interval_set_t result = network->output();

    if (result.empty()) {
      buffer << network->current << ",";
      for (auto const& [k, v] : network->prevargs) {
        if (k != "time") {
          buffer << v << ",";
        }
      }
      buffer << "0" << std::endl;
    }

    for (const auto& intv : result) {
      if (intv.lower() > network->previous) {
        t = intv.upper();
        buffer << intv.lower() << ",";
        for (auto const& [k, v] : network->prevargs) {
          if (k != "time") {
            buffer << v << ",";
          }
        }
        buffer << "0" << std::endl;

        buffer << intv.upper() << ",";
        for (auto const& [k, v] : network->prevargs) {
          if (k != "time") {
            buffer << v << ",";
          }
        }
        buffer << "1" << std::endl;

      } else {
        t = intv.upper();
        buffer << intv.upper() << ",";
        for (auto const& [k, v] : network->prevargs) {
          if (k != "time") {
            buffer << v << ",";
          }
        }
        buffer << "1" << std::endl;
      }
    }

    if (t < network->current) {
      buffer << network->current << ",";
      for (auto const& [k, v] : network->prevargs) {
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
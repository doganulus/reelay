/*
 *                 |
 *    __| _ \  _ \ |  _` | |   |
 *   |    __/  __/ | (   | |   |
 *  _|  \___|\___|_|\__,_|\__, |
 *                         ____/    Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include <memory>
#include <string>

#include "reelay/json.hpp"
#include "reelay/monitors/json_monitors/discrete_timed_condensing_json_monitor.hpp"
#include "reelay/monitors/json_monitors/discrete_timed_data_condensing_json_monitor.hpp"
#include "reelay/monitors/json_monitors/untimed_condensing_json_monitor.hpp"
#include "reelay/monitors/json_monitors/untimed_data_condensing_json_monitor.hpp"

#include "reelay/monitors/base_monitor.hpp"
#include "reelay/monitors/public_interface.hpp"

#include "reelay/parser/ptl.hpp"
#include "reelay/parser/ptl_inspector.hpp"

namespace reelay {

template <typename TimeT>
template <typename InputT, typename OutputT>
struct condensing<TimeT>::monitor {
  using time_t = TimeT;
  using input_t = InputT;
  using output_t = OutputT;

  using base_monitor_t = base_monitor<time_t, input_t, output_t>;
  using base_ptr_t = std::shared_ptr<base_monitor_t>;

  static base_ptr_t make(const std::string &pattern,
                         const reelay::kwargs &kw = reelay::kwargs()) {
    auto inspector = reelay::ptl_inspector();
    auto knowledge = inspector.inspect(pattern);

    bool timed = reelay::any_cast<bool>(knowledge["timed"]);
    bool categorical = reelay::any_cast<bool>(knowledge["has_references"]);

    if (not timed and not categorical) {
      return std::make_shared<
          untimed_condensing_json_monitor<time_t, input_t, output_t>>(
          pattern, kw);
    } else if (timed and not categorical) {
      return std::make_shared<
          discrete_timed_condensing_json_monitor<time_t, input_t, output_t>>(
          pattern, kw);
    } else if (not timed and categorical) {
      return std::make_shared<
          untimed_data_condensing_json_monitor<time_t, input_t, output_t>>(
          pattern, kw);
    } else {
      return std::make_shared<discrete_timed_data_condensing_json_monitor<
          time_t, input_t, output_t>>(pattern, kw);
    }
  }
};

}  // namespace reelay
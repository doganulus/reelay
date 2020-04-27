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

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "reelay/common.hpp"
#include "reelay/json.hpp"
#include "reelay/json_monitors/base_json_monitor.hpp"
#include "reelay/json_monitors/discrete_timed_data_json_monitor.hpp"
#include "reelay/json_monitors/discrete_timed_json_monitor.hpp"
#include "reelay/json_monitors/untimed_data_json_monitor.hpp"
#include "reelay/json_monitors/untimed_json_monitor.hpp"
#include "reelay/parser/ptl.hpp"
#include "reelay/parser/ptl_inspector.hpp"

namespace reelay {

template <typename T>
struct discrete_timed {
  using time_t = T;
  using input_t = json;
  using output_t = json;

  using monitor_t = base_json_monitor<time_t>;
  using monitor_ptr_t = std::shared_ptr<monitor_t>;

  static monitor_ptr_t monitor(
      const std::string &pattern, const reelay::kwargs &kw = reelay::kwargs()) {
    auto inspector = reelay::ptl_inspector();
    auto knowledge = inspector.inspect(pattern);

    bool timed = reelay::any_cast<bool>(knowledge["timed"]);
    bool categorical = reelay::any_cast<bool>(knowledge["has_references"]);

    if (not timed and not categorical) {
      return std::make_shared<untimed_json_monitor<time_t>>(pattern, kw);
    } else if (timed and not categorical) {
      return std::make_shared<discrete_timed_json_monitor<time_t>>(pattern, kw);
    } else if (not timed and categorical) {
      return std::make_shared<untimed_data_json_monitor<time_t>>(pattern, kw);
    } else {
      return std::make_shared<discrete_timed_data_json_monitor<time_t>>(pattern,
                                                                        kw);
    }
  }
};

}  // namespace reelay
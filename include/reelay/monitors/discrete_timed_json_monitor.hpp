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

#include "boost/variant.hpp"

#include "reelay/json.hpp"
#include "reelay/monitors/json_monitors/untimed_json_monitor.hpp"
#include "reelay/monitors/json_monitors/discrete_timed_json_monitor.hpp"
#include "reelay/monitors/json_monitors/untimed_data_json_monitor.hpp"
#include "reelay/monitors/json_monitors/discrete_timed_data_json_monitor.hpp"

#include "reelay/monitors/public_interface.hpp"

#include "reelay/parser/ptl.hpp"
#include "reelay/parser/ptl_inspector.hpp"

namespace reelay {

template <typename T>
struct discrete_timed<T>::json_monitor {
  using time_t = T;
  using input_t = json;
  using output_t = json;

  boost::variant<untimed_json_monitor<time_t>,
                 untimed_data_json_monitor<time_t>,
                 discrete_timed_json_monitor<time_t>,
                 discrete_timed_data_json_monitor<time_t>>         
      _impl;

  explicit json_monitor(const std::string &pattern,
                        const reelay::kwargs &kw = reelay::kwargs()) {
    auto inspector = reelay::ptl_inspector();
    auto knowledge = inspector.inspect(pattern);

    bool timed = reelay::any_cast<bool>(knowledge["timed"]);
    bool categorical = reelay::any_cast<bool>(knowledge["has_references"]);

    if (not timed and not categorical) {
      _impl = untimed_json_monitor<time_t>(pattern, kw);
    } else if (timed and not categorical) {
      _impl = discrete_timed_json_monitor<time_t>(pattern, kw);
    } else if (not timed and categorical) {
      _impl = untimed_data_json_monitor<time_t>(pattern, kw);
    } else {
      _impl = discrete_timed_data_json_monitor<time_t>(pattern, kw);
    }
  }

  struct visitor_update : boost::static_visitor<output_t> {
    output_t operator()(untimed_json_monitor<time_t> &monitor,
                        const input_t &object) const {
      return monitor.update(object);
    }
    output_t operator()(untimed_data_json_monitor<time_t> &monitor,
                        const input_t &object) const {
      return monitor.update(object);
    }
    output_t operator()(discrete_timed_json_monitor<time_t> &monitor,
                        const input_t &object) const {
      return monitor.update(object);
    }
    output_t operator()(discrete_timed_data_json_monitor<time_t> &monitor,
                        const input_t &object) const {
      return monitor.update(object);
    }
  };

  struct visitor_now : boost::static_visitor<time_t> {
    time_t operator()(untimed_json_monitor<time_t> &monitor) const {
      return monitor.now();
    }
    time_t operator()(untimed_data_json_monitor<time_t> &monitor) const {
      return monitor.now();
    }
    time_t operator()(
        discrete_timed_json_monitor<time_t> &monitor) const {
      return monitor.now();
    }
    time_t operator()(
        discrete_timed_data_json_monitor<time_t> &monitor) const {
      return monitor.now();
    }
  };

  output_t update(const input_t &object) {
    return boost::apply_visitor(visitor_update{}, _impl,
                                boost::variant<input_t>(object));
  }

  time_t now() { return boost::apply_visitor(visitor_now{}, _impl); }
};

}  // namespace reelay
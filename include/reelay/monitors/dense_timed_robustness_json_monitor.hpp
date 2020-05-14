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
#include "reelay/monitors/json_monitors/dense_timed_robustness_json_monitor.hpp"

#include "reelay/monitors/base_monitor.hpp"
#include "reelay/monitors/public_interface.hpp"

#include "reelay/parser/ptl.hpp"
#include "reelay/parser/ptl_inspector.hpp"

#include "reelay/constants.hpp"

namespace reelay {

template <typename TimeT, piecewise InterpolationT>
template <typename ValueT>
template <typename InputT, typename OutputT>
struct dense_timed<TimeT, InterpolationT>::robustness<ValueT>::monitor {
  using time_t = TimeT;
  using value_t = ValueT;
  using input_t = InputT;
  using output_t = OutputT;

  using base_monitor_t = base_monitor<time_t, input_t, output_t>;
  using base_ptr_t = std::shared_ptr<base_monitor_t>;

  static base_ptr_t make(const std::string &pattern,
                         const reelay::kwargs &kw = reelay::kwargs()) {
    auto inspector = reelay::ptl_inspector();
    auto knowledge = inspector.inspect(pattern);

    // bool timed = reelay::any_cast<bool>(knowledge["timed"]);
    bool categorical = reelay::any_cast<bool>(knowledge["has_references"]);

    if (not categorical and InterpolationT == piecewise::CONSTANT) {
      return std::make_shared<dense_timed_robustness_json_monitor<
          time_t, value_t, input_t, output_t>>(pattern, kw);
    } else if (not categorical and InterpolationT == piecewise::LINEAR) {
      throw std::invalid_argument((
        "Robustness semantics is currently not available "
        "for piecewise linear interpolation. "
        "Consider piecewise::CONSTANT option... "));
    } else if (categorical) {
      throw std::invalid_argument((
        "Robustness semantics is not available for specifications that "
        "contains data references. "));
    } else {
      throw std::invalid_argument((
        "Unknown option for interpolation order."));
    }
  }
};

}  // namespace reelay
/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "reelay/pybind11.hpp"
//
#include "reelay/formatters/formatter.hpp"

namespace reelay {

template <typename TimeT, typename ValueT>
struct discrete_timed_formatter<TimeT, ValueT, pybind11::object, true> {
  using time_t = TimeT;
  using value_t = ValueT;
  using output_t = pybind11::object;

  using interval = reelay::interval<time_t>;
  using interval_set = reelay::interval_set<time_t>;

  value_t lastval = std::numeric_limits<value_t>::lowest();

  std::string t_name;
  std::string y_name;

  explicit discrete_timed_formatter(
      const std::string& t_str = "time", const std::string& y_str = "value")
      : t_name(t_str), y_name(y_str) {}

  explicit discrete_timed_formatter(const basic_options& options)
      : discrete_timed_formatter(
          options.get_time_field_name(), options.get_value_field_name()) {}

  inline output_t now(time_t now) {
    return pybind11::dict(pybind11::arg(t_name.c_str()) = now);
  }

  output_t format(value_t result, time_t now) {
    if (result != lastval or now == 0) {
      lastval = result;
      return pybind11::dict(
          pybind11::arg(t_name.c_str()) = now,
          pybind11::arg(y_name.c_str()) = result);
    } else {
      lastval = result;
      return pybind11::dict();
    }
  }
};

}  // namespace reelay
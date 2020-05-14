/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "reelay/pybind11.hpp"

namespace reelay {

template <
    typename TimeT, typename ValueT = bool, typename OutputT = pybind11::object>
struct discrete_timed_python_formatter {
  using time_t = TimeT;
  using value_t = ValueT;
  using output_t = OutputT;

  using interval = reelay::interval<time_t>;
  using interval_set = reelay::interval_set<time_t>;

  value_t lastval = std::numeric_limits<value_t>::lowest();

  const std::string t_name;
  const std::string y_name;

  explicit discrete_timed_python_formatter(const std::string& t_str = "time",
                                           const std::string& y_str = "value")
      : t_name(t_str), y_name(y_str) {}

  output_t format(value_t result) {
    return pybind11::dict(pybind11::arg(y_name.c_str()) = result);
  }
};

}  // namespace reelay
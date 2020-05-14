/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

 #pragma once

#include "reelay/intervals.hpp"
#include "reelay/pybind11.hpp"

namespace reelay {

template <typename TimeT, typename ValueT, typename OutputT = pybind11::list>
struct dense_timed_robustness_python_formatter {
  using time_t = TimeT;
  using value_t = ValueT;
  using output_t = OutputT;

  using interval = reelay::interval<time_t>;
  using interval_map = reelay::robustness_interval_map<time_t, value_t>;

  const std::string t_name;
  const std::string y_name;

  value_t lastval = false;

  explicit dense_timed_robustness_python_formatter(
      const std::string& t_str = "time", const std::string& y_str = "value")
      : t_name(t_str), y_name(y_str) {}

  output_t format(const interval_map& result, time_t previous, time_t now){
    output_t vresult = pybind11::list();

    for (const auto& intv : result) {
      if (lastval != intv.second or now == 0) {
        vresult.append(pybind11::dict(
            pybind11::arg(t_name.c_str()) = intv.first.lower(),
            pybind11::arg(y_name.c_str()) = intv.second));
        lastval = intv.second;
      }
    }
    
    return vresult;
  }
};

} //namespace reelay
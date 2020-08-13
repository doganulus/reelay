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
#include "reelay/intervals.hpp"

namespace reelay {

template <typename TimeT, typename ValueT>
struct dense_timed_robustness_formatter<TimeT, ValueT, pybind11::object> {
  using time_t = TimeT;
  using value_t = ValueT;
  using output_t = pybind11::object;

  using interval = reelay::interval<time_t>;
  using interval_map = reelay::robustness_interval_map<time_t, value_t>;

  std::string t_name;
  std::string y_name;

  value_t lastval = false;

  explicit dense_timed_robustness_formatter(
      const std::string& t_str = "time", const std::string& y_str = "value")
      : t_name(t_str), y_name(y_str) {}

  explicit dense_timed_robustness_formatter(const basic_options& options)
      : dense_timed_robustness_formatter(
          options.get_time_field_name(), options.get_value_field_name()) {}

  inline output_t now(time_t now) {
    return pybind11::dict(pybind11::arg(t_name.c_str()) = now);
  }

  output_t format(const interval_map& result, time_t previous, time_t now){
    auto vresult = pybind11::list();

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
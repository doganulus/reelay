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
struct dense_timed_data_formatter<TimeT, ValueT, pybind11::object> {
  using time_t = TimeT;
  using value_t = ValueT;
  using output_t = pybind11::object;

  using interval = reelay::interval<time_t>;
  using interval_map = reelay::data_interval_map<time_t>;

  data_mgr_t manager;

  std::string t_name;
  std::string y_name;
 
  bool lastval = false;

  explicit dense_timed_data_formatter(
      const data_mgr_t& mgr, const std::string& t_str = "time",
      const std::string& y_str = "value")
      : manager(mgr), t_name(t_str), y_name(y_str) {}

  explicit dense_timed_data_formatter(const basic_options& options)
      : dense_timed_data_formatter(
          options.get_data_manager(), options.get_time_field_name(),
          options.get_value_field_name()) {}

  inline output_t now(time_t now) {
    return pybind11::dict(pybind11::arg(t_name.c_str()) = now);
  }

  inline output_t format(
      const interval_map& result, time_t previous, time_t now) {
    auto vresult = pybind11::list();

    for (const auto& intv : result) {
      bool value = (intv.second != manager->zero());
      if (lastval != value or now == 0) {
        vresult.append(pybind11::dict(
            pybind11::arg(t_name.c_str()) = intv.first.lower(),
            pybind11::arg(y_name.c_str()) = value));
        lastval = value;
      }
    }

    return vresult;
  }
};

} //namespace reelay
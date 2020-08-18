/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

 #pragma once

#include "reelay/formatters/formatter.hpp"
#include "reelay/intervals.hpp"
#include "reelay/json.hpp"
#include "reelay/unordered_data.hpp"
//
#include "reelay/options.hpp"

namespace reelay {

template <typename TimeT>
struct dense_timed_data_formatter<TimeT, bool, json> {
  using time_t = TimeT;
  using value_t = bool;
  using output_t = json;

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
    return json({{t_name, now}});
  }

  inline output_t format(const interval_map& result,
                  time_t previous, time_t now) {
    output_t vresult;
    for (const auto& intv : result) {
      bool value = (intv.second != manager->zero());
      if (lastval != value or now == 0) {
        vresult.push_back(json({{t_name, intv.first.lower()}, {y_name, value}}));
        lastval = value;
      }
    }
    return vresult;
  }
};

} //namespace reelay
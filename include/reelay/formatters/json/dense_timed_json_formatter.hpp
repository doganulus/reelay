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
//
#include "reelay/options.hpp"

namespace reelay {

template <typename TimeT>
struct dense_timed_formatter<TimeT, bool, json> {
  using time_t = TimeT;
  using value_t = bool;
  using output_t = json;

  using interval = reelay::interval<time_t>;
  using interval_set = reelay::interval_set<time_t>;

  std::string t_name;
  std::string y_name;
 
  bool lastval = false;

  explicit dense_timed_formatter(
      const std::string& t_str = "time", const std::string& y_str = "value")
      : t_name(t_str), y_name(y_str) {}

  explicit dense_timed_formatter(const basic_options& options)
      : dense_timed_formatter(
          options.get_time_field_name(), options.get_value_field_name()) {}

  inline output_t now(time_t now) {
    return json({{t_name, now}});
  }

  output_t format(const interval_set& result, time_t previous, time_t now) {
    if (now == 0) {
      return _init_0(result, previous, now);
    } else if (previous == 0) {
      return _init_1(result, previous, now);
    } else {
      return _format(result, previous, now);
    }
  }

  output_t _init_0(const interval_set& result, time_t previous, time_t now) {
    // This code takes care of the special case of time zero
    // HERE: time_t now = 0
    return std::vector<reelay::json>();
  }

  output_t _init_1(const interval_set& result, time_t previous, time_t now) {
    // This code takes care of the special case of the first segment
    // The variable `lastval` is meaningless for the first segment
    // HERE: The first segment ranges from previous=0 to now

    auto vresult = std::vector<reelay::json>();

    if (result.empty()) {
      vresult.push_back(json({{t_name, previous}, {y_name, false}}));
      lastval = false;
    } else {
      if (result.begin()->lower() != previous) {
        vresult.push_back(json({{t_name, previous}, {y_name, false}}));
        lastval = false;
      }
      for (const auto& intv : result) {
        vresult.push_back(json({{t_name, intv.lower()}, {y_name, true}}));
        lastval = true;
        if (intv.upper() != now) {
          vresult.push_back(json({{t_name, intv.upper()}, {y_name, false}}));
          lastval = false;
        }
      }
    }

    return vresult;
  }

  inline output_t _format(
      const interval_set& result, time_t previous, time_t now) {
    // This code constitutes the main operation such that
    //   + do output two events per interval in the set
    //   + take care empty interval set (meaning all false)
    //   + do not output if it is the same value with the last value (lastval)
    //   + do not output for the current point (now) as it may be not finished

    auto vresult = std::vector<reelay::json>();

    if (result.empty()) {
      if (lastval) {
        vresult.push_back(json({{t_name, previous}, {y_name, false}}));
        lastval = false;
      }
    } else {
      for (const auto& intv : result) {
        if (not lastval) {
          vresult.push_back(json({{t_name, intv.lower()}, {y_name, true}}));
          lastval = true;
        }
        if (intv.upper() != now) {
          vresult.push_back(json({{t_name, intv.upper()}, {y_name, false}}));
          lastval = false;
        }
      }
    }

    return vresult;
  }
};

} //namespace reelay
/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "reelay/formatters/formatter.hpp"
#include "reelay/json.hpp"
#include "reelay/intervals.hpp"
//
#include "reelay/options.hpp"

namespace reelay {

template <typename TimeT, typename ValueT>
struct discrete_timed_formatter<TimeT, ValueT, json, true> {
  using time_t = TimeT;
  using value_t = ValueT;
  using output_t = json;

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
    return json({{t_name, now}});
  }

  inline output_t format(value_t result, time_t now) {
    if (result != lastval or now == 0) {
      lastval = result;
      return json({{t_name, now}, {y_name, result}});
    } else {
      lastval = result;
      return json({});  // Empty JSON object {}
    }
  }
};

}  // namespace reelay
/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "reelay/intervals.hpp"
#include "reelay/json.hpp"

namespace reelay {

template <
    typename TimeT, typename ValueT = bool,
    typename OutputT = std::vector<reelay::json>>
struct dense_timed_data_json_formatter {
  using time_t = TimeT;
  using value_t = ValueT;
  using output_t = OutputT;

  using interval = reelay::interval<time_t>;
  using interval_map = reelay::data_interval_map<time_t>;

  std::string t_name;
  std::string y_name;

  value_t lastval = false;

  explicit dense_timed_data_json_formatter(const std::string t_str = "time",
                                         const std::string y_str = "value")
      : t_name(t_str), y_name(y_str) {}

  inline output_t format(const data_mgr_t& mgr, const interval_map& result,
                  time_t previous, time_t now) {
    output_t vresult;
    for (const auto& intv : result) {
      bool value = (intv.second == mgr->one());
      if (lastval != value or now == 0) {
        vresult.push_back(json({{t_name, intv.first.lower()}, {y_name, value}}));
        lastval = value;
      }
    }
    return vresult;
  }
};

}  // namespace reelay
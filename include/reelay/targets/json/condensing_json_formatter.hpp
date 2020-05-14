/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "reelay/json.hpp"

namespace reelay {

template <typename TimeT, typename ValueT = bool, typename OutputT = json>
struct condensing_json_formatter {
  using time_t = TimeT;
  using value_t = ValueT;
  using output_t = OutputT;

  value_t lastval = false;

  std::string t_name;
  std::string y_name;

  explicit condensing_json_formatter(const std::string& t_str = "time",
                             const std::string& y_str = "value")
      : t_name(t_str), y_name(y_str) {}

  inline output_t format(value_t result, time_t now) {
    if (result != lastval or now == 0) {
      lastval = result;
      return json({{t_name, now}, {y_name, result}});
    } else {
      lastval = result;
      return json({}); // Empty JSON object {}
    }
  }
};

}  // namespace reelay
/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <type_traits> 
#include "reelay/json.hpp"

namespace reelay {

template <typename TimeT, typename ValueT = bool, typename OutputT = json>
struct discrete_timed_json_formatter {
  using time_t = TimeT;
  using value_t = ValueT;
  using output_t = OutputT;

  // std::static_assert(std::is_integral<time_t>::value,
  //               "Discrete time type must be an integral type.");

  std::string t_name;
  std::string y_name;

  explicit discrete_timed_json_formatter(const std::string& t_str = "time",
                                         const std::string& y_str = "value")
      : t_name(t_str), y_name(y_str) {}

  inline output_t format(value_t result) { return json({{y_name, result}}); }
};

}  // namespace reelay
/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include <memory>
#include <string>
#include <type_traits>

#include "reelay/common.hpp"
#include "reelay/json.hpp"
#include "reelay/networks.hpp"
#include "reelay/parser/ptl.hpp"
#include "reelay/settings.hpp"

#include "reelay/monitors/base_monitor.hpp"
#include "reelay/targets/json/condensing_json_formatter.hpp"

namespace reelay {

template <
    typename TimeT, typename InputT = reelay::json, typename OutputT = InputT,
    class FormatterT = condensing_json_formatter<TimeT, OutputT>>
struct discrete_timed_condensing_json_monitor final
    : base_monitor<TimeT, InputT, OutputT> {
  using time_t = TimeT;
  using input_t = InputT;
  using output_t = OutputT;

  using factory = discrete_timed_setting::factory<input_t, time_t>;

  using network_t = typename factory::network_t;
  using network_ptr_t = typename factory::network_ptr_t;

  using formatter_t = FormatterT;

  // std::static_assert(not std::is_integral<time_t>::value,
  //                    "Discrete time type must be an integral type.");

  network_ptr_t network;
  formatter_t formatter = formatter_t();

  discrete_timed_condensing_json_monitor() = default;

  explicit discrete_timed_condensing_json_monitor(const std::string &pattern,
                                                  const reelay::kwargs &kw
                                                  = reelay::kwargs()) {
    auto parser = ptl_parser<factory>(kw);
    this->network = parser.parse(pattern);

    try {
      formatter.t_name = reelay::any_cast<std::string>(kw.at("t_name"));
    } catch (const std::out_of_range &oor) {
    }

    try {
      formatter.y_name = reelay::any_cast<std::string>(kw.at("y_name"));
    } catch (const std::out_of_range &oor) {
    }
  }

  output_t update(const input_t &args) {
    this->network->update(args);
    return formatter.format(network->output(), network->now());
  }

  time_t now() { return network->now(); }
};
}  // namespace reelay
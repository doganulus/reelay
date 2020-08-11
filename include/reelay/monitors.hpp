/*
 *                 |
 *    __| _ \  _ \ |  _` | |   |
 *   |    __/  __/ | (   | |   |
 *  _|  \___|\___|_|\__,_|\__, |
 *                         ____/    Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include <exception>
//
#include "reelay/parser/ptl_inspector.hpp"
//
#include "reelay/formatters/json_formatter.hpp"
#include "reelay/networks.hpp"
//
#include "reelay/monitors/monitor.hpp"
//
#include "reelay/monitors/dense_timed_data_monitor.hpp"
#include "reelay/monitors/dense_timed_monitor.hpp"
#include "reelay/monitors/dense_timed_robustness_0_monitor.hpp"
#include "reelay/monitors/discrete_timed_data_monitor.hpp"
#include "reelay/monitors/discrete_timed_monitor.hpp"
#include "reelay/monitors/discrete_timed_robustness_monitor.hpp"
//
#include "reelay/options.hpp"

namespace reelay {


// Helper structs
template <typename TimeT>
struct dense_timed {

  template <typename InputT, typename OutputT = json>
  struct monitor {
    static dense_monitor_options<TimeT, InputT, OutputT> options() {
      return dense_monitor_options<TimeT, InputT, OutputT>();
    }
  };

  template <typename ValueT>
  struct robustness {
    template <typename InputT, typename OutputT = json>
    struct monitor {
      static dense_robustness_monitor_options<TimeT, ValueT, InputT, OutputT>
      options() {
        return dense_robustness_monitor_options<TimeT, ValueT, InputT, OutputT>();
      }
    };
  };
};

template <typename TimeT>
struct discrete_timed {

  template <typename InputT, typename OutputT = json>
  struct monitor {
    static discrete_monitor_options<TimeT, InputT, OutputT> options() {
      return discrete_monitor_options<TimeT, InputT, OutputT>();
    }
  };

  template <typename ValueT>
  struct robustness {
    template <typename InputT, typename OutputT = json>
    struct monitor {
      static discrete_robustness_monitor_options<TimeT, ValueT, InputT, OutputT>
      options() {
        return discrete_robustness_monitor_options<TimeT, ValueT, InputT, OutputT>();
      }
    };
  };
};

template <typename TimeT, typename InputT, typename OutputT>
static monitor<InputT, OutputT> make_monitor(
    const std::string& pattern,
    const dense_monitor_options<TimeT, InputT, OutputT>& options) {
  using time_type = TimeT;
  using input_type = InputT;
  using output_type = OutputT;

  using type = monitor<InputT, OutputT>;

  auto inspector = reelay::ptl_inspector();
  auto inspection = inspector.inspect(pattern);

  if (reelay::any_cast<bool>(inspection["has_references"])) {
    return type(dense_timed_data_monitor<time_type, input_type, output_type>::
                    make_shared(pattern, options.get_basic_options()));

  } else {
    return type(
        dense_timed_monitor<time_type, input_type, output_type>::make_shared(
            pattern, options.get_basic_options()));
  }
}

template <typename TimeT, typename ValueT, typename InputT, typename OutputT>
static monitor<InputT, OutputT> make_monitor(
    const std::string& pattern,
    const dense_robustness_monitor_options<TimeT, ValueT, InputT, OutputT>&
        options) {
  using time_type = TimeT;
  using value_type = ValueT;
  using input_type = InputT;
  using output_type = OutputT;

  using type = monitor<InputT, OutputT>;

  auto inspector = reelay::ptl_inspector();
  auto inspection = inspector.inspect(pattern);

  if (not reelay::any_cast<bool>(inspection["has_references"])) {
    return type(
        dense_timed_robustness_0_monitor<
            time_type, value_type, input_type,
            output_type>::make_shared(pattern, options.get_basic_options()));
  } else {
    throw std::invalid_argument(
        "Error: Data references are not supported for robustness settings.");
  }
}

template <typename TimeT, typename InputT, typename OutputT>
static monitor<InputT, OutputT> make_monitor(
    const std::string& pattern,
    const discrete_monitor_options<TimeT, InputT, OutputT>& options) {
  using time_type = TimeT;
  using input_type = InputT;
  using output_type = OutputT;

  using type = monitor<InputT, OutputT>;

  auto inspector = reelay::ptl_inspector();
  auto inspection = inspector.inspect(pattern);

  if (reelay::any_cast<bool>(inspection["has_references"])
      and options.is_condensing()) {
    return type(
        discrete_timed_data_monitor<time_type, input_type, output_type, true>::
            make_shared(pattern, options.get_basic_options()));

  } else if (
      reelay::any_cast<bool>(inspection["has_references"])
      and not options.is_condensing()) {
    return type(
        discrete_timed_data_monitor<time_type, input_type, output_type, false>::
            make_shared(pattern, options.get_basic_options()));
  } else if (
      not reelay::any_cast<bool>(inspection["has_references"])
      and options.is_condensing()) {
    return type(
        discrete_timed_monitor<time_type, input_type, output_type, true>::
            make_shared(pattern, options.get_basic_options()));
  } else {
    return type(
        discrete_timed_monitor<time_type, input_type, output_type, false>::
            make_shared(pattern, options.get_basic_options()));
  }
}

template <typename TimeT, typename ValueT, typename InputT, typename OutputT>
static monitor<InputT, OutputT> make_monitor(
    const std::string& pattern,
    const discrete_robustness_monitor_options<TimeT, ValueT, InputT, OutputT>&
        options) {
  using time_type = TimeT;
  using value_type = ValueT;
  using input_type = InputT;
  using output_type = OutputT;

  using type = monitor<InputT, OutputT>;

  auto inspector = reelay::ptl_inspector();
  auto inspection = inspector.inspect(pattern);

  if (not reelay::any_cast<bool>(inspection["has_references"])
      and options.is_condensing()) {
    return type(
        discrete_timed_robustness_monitor<
            time_type, value_type, input_type,
            output_type, true>::make_shared(pattern, options.get_basic_options()));
  } else if (
      not reelay::any_cast<bool>(inspection["has_references"])
      and not options.is_condensing()) {
    return type(
        discrete_timed_robustness_monitor<
            time_type, value_type, input_type,
            output_type, false>::make_shared(pattern, options.get_basic_options()));
  } else {
    throw std::invalid_argument(
        "Error: Data references are not supported for robustness settings.");
  }
  }

}  // namespace reelay
/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "string"

#include "reelay/common.hpp"
#include "reelay/intervals.hpp"
#include "reelay/networks/basic_structure.hpp"

namespace reelay {
namespace dense_timed_robustness_0_setting {

template <typename X, typename V, typename T>
struct record0 : public dense_timed_state<X, robustness_interval_map<T, V>, T> {
  using time_t = T;
  using input_t = X;
  using value_t = V;
  using output_t = reelay::robustness_interval_map<time_t, value_t>;

  using interval = reelay::interval<time_t>;
  using interval_map = reelay::robustness_interval_map<time_t, value_t>;

  using node_t = dense_timed_node<output_t, time_t>;
  using state_t = dense_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  using function_t =
      std::function<output_t(const input_t &, const input_t &, time_t, time_t)>;

  std::vector<function_t> funcs = {};

  interval_map value = interval_map();

  explicit record0(const std::vector<
                   std::pair<std::string, std::pair<std::string, std::string>>>
                       &fields) {

    for (std::size_t i = 0; i < fields.size(); i++) {
      auto key_name = fields[i].first;
      auto field_value_type = fields[i].second.first;
      auto field_value_str = fields[i].second.second;
      if (field_value_type == "bool" and field_value_str == "true") {
        funcs.push_back([key_name](const input_t &, const input_t &args,
                                   time_t previous, time_t now) {
          if (field_of<input_t>::as_bool(args, key_name)) {
            return interval_map(
                std::make_pair(interval::left_open(previous, now),
                               reelay::infinity<value_t>::value()));
          } else {
            return interval_map(
                std::make_pair(interval::left_open(previous, now),
                               -reelay::infinity<value_t>::value()));
          }
        });
      } else if (field_value_type == "bool" and field_value_str == "false") {
        funcs.push_back([key_name](const input_t &, const input_t &args,
                                   time_t previous, time_t now) {
          if (not field_of<input_t>::as_bool(args, key_name)) {
            return interval_map(
                std::make_pair(interval::left_open(previous, now),
                               reelay::infinity<value_t>::value()));
          } else {
            return interval_map(
                std::make_pair(interval::left_open(previous, now),
                               -reelay::infinity<value_t>::value()));
          }
        });
      } else if (field_value_type == "string") {
        funcs.push_back([key_name,
                         field_value_str](const input_t &, const input_t &args,
                                          time_t previous, time_t now) {
          if (field_of<input_t>::as_string(args, key_name) == field_value_str) {
            return interval_map(
                std::make_pair(interval::left_open(previous, now),
                               reelay::infinity<value_t>::value()));
          } else {
            return interval_map(
                std::make_pair(interval::left_open(previous, now),
                               -reelay::infinity<value_t>::value()));
          }
        });
      } else if (field_value_type == "eq") {
        throw std::runtime_error("Syntax Error: Operator== are not "
                                 "supported for the dense timed setting.");
      } else if (field_value_type == "ne") {
        throw std::runtime_error("Syntax Error: Operator!= are not "
                                 "supported for the dense timed setting.");
      } else if (field_value_type == "ge") {
        double c = boost::lexical_cast<double>(field_value_str);
        funcs.push_back([key_name, c](const input_t &, const input_t &args,
                                      time_t previous, time_t now) {
          return interval_map(
              std::make_pair(interval::left_open(previous, now),
                             field_of<input_t>::as_float(args, key_name) - c));
        });
      } else if (field_value_type == "gt") {
        double c = boost::lexical_cast<double>(field_value_str);
        funcs.push_back([key_name, c](const input_t &, const input_t &args,
                                      time_t previous, time_t now) {
          return interval_map(
              std::make_pair(interval::left_open(previous, now),
                             field_of<input_t>::as_float(args, key_name) - c));
        });
      } else if (field_value_type == "le") {
        double c = boost::lexical_cast<double>(field_value_str);
        funcs.push_back([key_name, c](const input_t &, const input_t &args,
                                      time_t previous, time_t now) {
          return interval_map(
              std::make_pair(interval::left_open(previous, now),
                             c - field_of<input_t>::as_float(args, key_name)));
        });
      } else if (field_value_type == "lt") {
        double c = boost::lexical_cast<double>(field_value_str);
        funcs.push_back([key_name, c](const input_t &, const input_t &args,
                                      time_t previous, time_t now) {
          return interval_map(
              std::make_pair(interval::left_open(previous, now),
                             c - field_of<input_t>::as_float(args, key_name)));
        });
      } else if (field_value_type == "variable_ref") {
        throw std::runtime_error("Syntax Error: Variable references are not "
                                 "supported for the untimed setting.");
      } else if (field_value_type == "ignore_field") {
        throw std::runtime_error("Syntax Error: Variable references are not "
                                 "supported for the untimed setting.");
      } else {
        throw std::runtime_error(
            "Syntax Error: Unknown atomic field description.");
      }
    }
  }

  explicit record0(const kwargs &kw)
      : record0(
            reelay::any_cast<std::vector<
                std::pair<std::string, std::pair<std::string, std::string>>>>(
                kw.at("fields"))) {}

  void update(const input_t &pargs, const input_t &args, time_t previous,
              time_t now) override {        
    value = funcs[0](pargs, args, previous, now);

    for (size_t i = 1; i < funcs.size(); i++) {
      value = value - funcs[i](pargs, args, previous, now);
    }
  }
  output_t output(time_t, time_t) override { return value; }
};

} // namespace dense_timed_robustness_0_setting
} // namespace reelay
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
#include "reelay/networks/basic_structure.hpp"

namespace reelay {
namespace dense_timed_setting {

template <typename X, typename T>
struct record1 : public dense_timed_state<X, interval_set<T>, T> {
  using input_t = X;
  using time_t = T;

  using base_t = reelay::interval_set<time_t>;
  using output_t = reelay::interval_set<time_t>;

  using function_t =
      std::function<output_t(const input_t &, const input_t &, time_t, time_t)>;

  using interval = reelay::interval<time_t>;
  using interval_set = reelay::interval_set<time_t>;

  std::vector<function_t> funcs = {};

  output_t value = interval_set();

  explicit record1(const std::vector<
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
            return interval_set(interval::left_open(previous, now));
          } else {
            return interval_set();
          }
        });
      } else if (field_value_type == "bool" and field_value_str == "false") {
        funcs.push_back([key_name](const input_t &, const input_t &args,
                                   time_t previous, time_t now) {
          if (not field_of<input_t>::as_bool(args, key_name)) {
            return interval_set(interval::left_open(previous, now));
          } else {
            return interval_set();
          }
        });
      } else if (field_value_type == "string") {
        funcs.push_back([key_name,
                         field_value_str](const input_t &, const input_t &args,
                                          time_t previous, time_t now) {
          if (field_of<input_t>::as_string(args, key_name) == field_value_str) {
            return interval_set(interval::left_open(previous, now));
          } else {
            return interval_set();
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
        funcs.push_back([key_name, c](const input_t &pargs, const input_t &args,
                                      time_t previous, time_t now) {
          double y0 = field_of<input_t>::as_float(pargs, key_name);
          double y1 = field_of<input_t>::as_float(args, key_name);

          if (y0 < c and y1 < c) {
            return interval_set();
          } else if (y0 > c and y1 > c) {
            return interval_set(interval::left_open(previous, now));
          } else if (y0 > y1) {
            time_t crossing = now - (now - previous) * ((c - y1) / (y0 - y1));
            return interval_set(interval::left_open(previous, crossing));
          } else if (y0 < y1) {
            time_t crossing =
                previous + (now - previous) * ((c - y0) / (y1 - y0));
            return interval_set(interval::left_open(crossing, now));
          } else { // y0 == y1 == c is true
            return interval_set(interval::left_open(previous, now));
          }
        });
      } else if (field_value_type == "gt") {
        double c = boost::lexical_cast<double>(field_value_str);
        funcs.push_back([key_name, c](const input_t &pargs, const input_t &args,
                                      time_t previous, time_t now) {
          double y0 = field_of<input_t>::as_float(pargs, key_name);
          double y1 = field_of<input_t>::as_float(args, key_name);

          if (y0 < c and y1 < c) {
            return interval_set();
          } else if (y0 > c and y1 > c) {
            return interval_set(interval::left_open(previous, now));
          } else if (y0 > y1) {
            time_t crossing = now - (now - previous) * ((c - y1) / (y0 - y1));
            return interval_set(interval::left_open(previous, crossing));
          } else if (y0 < y1) {
            time_t crossing =
                previous + (now - previous) * ((c - y0) / (y1 - y0));
            return interval_set(interval::left_open(crossing, now));
          } else { // y0 == y1 == c is true
            return interval_set();
          }
        });
      } else if (field_value_type == "le") {
        double c = boost::lexical_cast<double>(field_value_str);
        funcs.push_back([key_name, c](const input_t &pargs, const input_t &args,
                                      time_t previous, time_t now) {
          double y0 = field_of<input_t>::as_float(pargs, key_name);
          double y1 = field_of<input_t>::as_float(args, key_name);

          if (y0 < c and y1 < c) {
            return interval_set(interval::left_open(previous, now));
          } else if (y0 > c and y1 > c) {
            return interval_set();
          } else if (y0 > y1) {
            time_t crossing = now - (now - previous) * ((c - y1) / (y0 - y1));
            return interval_set(interval::left_open(crossing, now));
          } else if (y0 < y1) {
            time_t crossing =
                previous + (now - previous) * ((c - y0) / (y1 - y0));
            return interval_set(interval::left_open(previous, crossing));
          } else { // y0 == y1 == c is true
            return interval_set(interval::left_open(previous, now));
          }
        });
      } else if (field_value_type == "lt") {
        double c = boost::lexical_cast<double>(field_value_str);
        funcs.push_back([key_name, c](const input_t &pargs, const input_t &args,
                                      time_t previous, time_t now) {
          double y0 = field_of<input_t>::as_float(pargs, key_name);
          double y1 = field_of<input_t>::as_float(args, key_name);

          if (y0 < c and y1 < c) {
            return interval_set(interval::left_open(previous, now));
          } else if (y0 > c and y1 > c) {
            return interval_set();
          } else if (y0 > y1) {
            time_t crossing = now - (now - previous) * ((c - y1) / (y0 - y1));
            return interval_set(interval::left_open(crossing, now));
          } else if (y0 < y1) {
            time_t crossing =
                previous + (now - previous) * ((c - y0) / (y1 - y0));
            return interval_set(interval::left_open(previous, crossing));
          } else { // y0 == y1 == c is true
            return interval_set();
          }
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

  explicit record1(const kwargs &kw)
      : record1(
            reelay::any_cast<std::vector<
                std::pair<std::string, std::pair<std::string, std::string>>>>(
                kw.at("fields"))) {}

  void update(const input_t &pargs, const input_t &args, time_t previous,
              time_t now) override {
    value = funcs[0](pargs, args, previous, now);
    for (std::size_t i = 1; i < funcs.size(); i++) {
      value &= funcs[i](pargs, args, previous, now);
    }
  }
  output_t output(time_t, time_t) override { return value; }
};

} // namespace dense_timed_setting
} // namespace reelay
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
namespace discrete_timed_robustness_setting {

template <typename X, typename V, typename T>
struct record : public discrete_timed_state<X, V, T> {
  using time_t = T;
  using input_t = X;
  using output_t = V;

  using function_t = std::function<output_t(const input_t &)>;

  function_t fn;

  std::vector<function_t> funcs = {};

  output_t value;

  explicit record(
      const std::vector<
          std::pair<std::string, std::pair<std::string, std::string>>>
          &fields) {

    for (std::size_t i = 0; i < fields.size(); i++) {
      auto key_name = fields[i].first;
      auto field_value_type = fields[i].second.first;
      auto field_value_str = fields[i].second.second;
      if (field_value_type == "bool" and field_value_str == "true") {
        funcs.push_back([key_name](const input_t &x) {
          if (field_of<input_t>::as_bool(x, key_name)) {
            return reelay::infinity<output_t>::value();
          } else {
            return -reelay::infinity<output_t>::value();
          }
        });
      } else if (field_value_type == "bool" and field_value_str == "false") {
        funcs.push_back([key_name](const input_t &x) {
          if (not field_of<input_t>::as_bool(x, key_name)) {
            return reelay::infinity<output_t>::value();
          } else {
            return -reelay::infinity<output_t>::value();
          }
        });
      } else if (field_value_type == "string") {
        funcs.push_back([key_name, field_value_str](const input_t &x) {
          if (field_of<input_t>::as_string(x, key_name) == field_value_str) {
            return reelay::infinity<output_t>::value();
          } else {
            return -reelay::infinity<output_t>::value();
          }
        });
      } else if (field_value_type == "eq") {
        throw std::runtime_error("Syntax Error: Operator!= are not "
                                 "supported for the robustness setting.");
      } else if (field_value_type == "ne") {
        throw std::runtime_error("Syntax Error: Operator!= are not "
                                 "supported for the robustness setting.");
      } else if (field_value_type == "ge") {
        float number = boost::lexical_cast<float>(field_value_str);
        funcs.push_back([key_name, number](const input_t &x) {
          return field_of<input_t>::as_float(x, key_name) - number;
        });
      } else if (field_value_type == "gt") {
        float number = boost::lexical_cast<float>(field_value_str);
        funcs.push_back([key_name, number](const input_t &x) {
          return field_of<input_t>::as_float(x, key_name) - number;
        });
      } else if (field_value_type == "le") {
        float number = boost::lexical_cast<float>(field_value_str);
        funcs.push_back([key_name, number](const input_t &x) {
          return number - field_of<input_t>::as_float(x, key_name);
        });
      } else if (field_value_type == "lt") {
        float number = boost::lexical_cast<float>(field_value_str);
        funcs.push_back([key_name, number](const input_t &x) {
          return number - field_of<input_t>::as_float(x, key_name);
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

  explicit record(const kwargs &kw)
      : record(
            reelay::any_cast<std::vector<
                std::pair<std::string, std::pair<std::string, std::string>>>>(
                kw.at("fields"))) {}

  void update(const input_t &args, time_t) override {
    value = funcs[0](args);
    for (std::size_t i = 1; i < funcs.size(); i++) {
      value &= funcs[i](args);
    }
  }
  output_t output(time_t) override { return value; }
};

} // namespace discrete_timed_robustness_setting
} // namespace reelay
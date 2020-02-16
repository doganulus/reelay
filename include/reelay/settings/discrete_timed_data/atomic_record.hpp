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
namespace discrete_timed_data_setting {

// template <typename X> struct record {};

template <typename X, typename T>
struct record : public discrete_timed_state<X, data_set_t, T> {
  explicit record(const kwargs &) {}
};

template <typename T>
struct record<std::unordered_map<std::string, std::string>, T>
    : public discrete_timed_state<std::unordered_map<std::string, std::string>,
                                  data_set_t, T> {
  using time_t = T;
  using value_t = data_set_t;
  using output_t = data_set_t;
  using input_t = std::unordered_map<std::string, std::string>;

  using function_t = std::function<output_t(const input_t &)>;

  data_mgr_t manager;

  std::vector<function_t> funcs = {};

  data_set_t value;

  explicit record(
      const data_mgr_t &mgr,
      const std::vector<
          std::pair<std::string, std::pair<std::string, std::string>>> &fields)
      : manager(mgr) {

    for (std::size_t i = 0; i < fields.size(); i++) {
      auto key_name = fields[i].first;
      auto field_value_type = fields[i].second.first;
      auto field_value_str = fields[i].second.second;
      if (field_value_type == "bool" and field_value_str == "true") {
        funcs.push_back([mgr, key_name](const input_t &x) {
          if (x.find(key_name) != x.end() and
              field_of<input_t>::as_bool(x, key_name)) {
            return mgr->one();
          } else {
            return mgr->zero();
          }
        });
      } else if (field_value_type == "bool" and field_value_str == "false") {
        funcs.push_back([mgr, key_name](const input_t &x) {
          if (x.find(key_name) != x.end() and
              field_of<input_t>::as_bool(x, key_name)) {
            return mgr->zero();
          } else {
            return mgr->one();
          }
        });
      } else if (field_value_type == "string") {
        funcs.push_back([mgr, key_name, field_value_str](const input_t &x) {
          if (x.find(key_name) != x.end() and
              field_of<input_t>::as_string(x, key_name) == field_value_str) {
            return mgr->one();
          } else {
            return mgr->zero();
          }
        });
      } else if (field_value_type == "variable_ref") {
        mgr->add_variable(field_value_str);
        funcs.push_back([mgr, key_name, field_value_str](const input_t &x) {
          if (x.find(key_name) != x.end()) {
            return mgr->assign(field_value_str, x.at(key_name));
          } else {
            return mgr->zero();
          }
        });
      } else if (field_value_type == "ignore_field") {
        funcs.push_back([mgr, key_name](const input_t &x) {
          if (x.find(key_name) != x.end()) {
            return mgr->one();
          } else {
            return mgr->zero();
          }
        });
      } else {
        throw std::runtime_error(
            "Syntax Error: Unknown atomic field description.");
      }
    }
  }

  explicit record(const kwargs &kw)
      : record(reelay::any_cast<data_mgr_t>(kw.at("manager")),
               reelay::any_cast <
                   std::vector<std::pair<std::string,
                                         std::pair<std::string, std::string>>>>(
                       kw.at("fields"))) {}

  void update(const input_t &args, time_t) override {
    value = funcs[0](args);
    for (std::size_t i = 1; i < funcs.size(); i++) {
      value *= funcs[i](args);
    }
  }
  output_t output(time_t) override { return value; }
};

} // namespace untimed_data_setting
} // namespace reelay
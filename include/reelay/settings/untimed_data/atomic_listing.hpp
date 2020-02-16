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
#include "reelay/unordered_data.hpp"

namespace reelay {
namespace untimed_data_setting {

template <typename X> 
struct listing : public untimed_state<X, data_set_t> {
  explicit listing(const kwargs &) {}
};

template <>
struct listing<std::vector<std::string>>
    : public untimed_state<std::vector<std::string>, data_set_t> {

  using input_t = std::vector<std::string>;
  using output_t = data_set_t;

  using function_t = std::function<data_set_t(const input_t &)>;

  data_mgr_t manager;

  std::vector<function_t> funcs = {};

  data_set_t value;

  explicit listing(
      const data_mgr_t &mgr,
      const std::vector<std::pair<std::string, std::string>> &fields)
      : manager(mgr) {
    for (std::size_t i = 0; i < fields.size(); i++) {
      auto field_value_type = fields[i].first;
      auto field_value_str = fields[i].second;
      if (field_value_type == "bool" and field_value_str == "true") {
        funcs.push_back([mgr, i](const input_t &x) {
          if (field_of<input_t>::as_bool(x, i)) {
            return mgr->one();
          } else {
            return mgr->zero();
          }
        });
      } else if (field_value_type == "bool" and field_value_str == "false") {
        funcs.push_back([mgr, i](const input_t &x) {
          if (field_of<input_t>::as_bool(x, i)) {
            return mgr->zero();
          } else {
            return mgr->one();
          }
        });
      } else if (field_value_type == "string") {
        funcs.push_back([mgr, field_value_str, i](const input_t &x) {
          if(field_of<input_t>::as_string(x, i) == field_value_str){
            return mgr->one();
          } else {
            return mgr->zero();
          }
        });
      } else if (field_value_type == "number") {
        float number = boost::lexical_cast<float>(field_value_str);
        funcs.push_back([mgr, number, i](const input_t &x) {
          if (field_of<input_t>::as_float(x, i) == number) {
            return mgr->one();
          } else {
            return mgr->zero();
          }
        });
      } else if (field_value_type == "variable_ref") {
        mgr->add_variable(field_value_str);
        funcs.push_back([mgr, field_value_str, i](const input_t &x) {
          return mgr->assign(field_value_str, x[i]);
        });
      } else if (field_value_type == "ignore_field") {
        funcs.push_back([mgr](const input_t &) { return mgr->one(); });
      } else {
        throw std::runtime_error(
            "Syntax Error: Unknown atomic field description.");
      }
    }
  }

  explicit listing(const kwargs &kw)
      : listing(reelay::any_cast<data_mgr_t>(kw.at("manager")),
                reelay::any_cast<std::vector<std::pair<std::string, std::string>>>(
                    kw.at("fields"))) {}

  void update(const input_t &args) override {
    if (args.size() != funcs.size()) {
      value = manager->zero();
    } else {
      value = manager->one();
      for (std::size_t i = 0; i < funcs.size(); i++) {
        value *= funcs[i](args);
      }
    }
  }
  output_t output() override { return value; }
};

} // namespace untimed_data_setting
} // namespace reelay
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

// template <typename X> struct record {};

template <typename X>
struct record : public untimed_state<X, data_set_t> {
  explicit record(const kwargs &) {}
};

template <>
struct record<std::unordered_map<std::string, std::string>>
    : public untimed_state<std::unordered_map<std::string, std::string>,
                           data_set_t> {

  using input_t = std::unordered_map<std::string, std::string>;
  using output_t = data_set_t;

  using function_t = std::function<data_set_t(const input_t &)>;
  
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
      auto value_type = fields[i].second.first;
      if (value_type == "proposition") {
        auto name = fields[i].second.second;
        funcs.push_back([mgr, key_name, name](const input_t &x) {
          if (x.find(key_name) != x.end() and name == x.at(key_name)) {
            return mgr->one();
          } else {
            return mgr->zero();
          }
        });
      } else if (value_type == "variable_ref") {
        auto name = fields[i].second.second;
        mgr->add_variable(name);
        funcs.push_back([mgr, key_name, name](const input_t &x) {
          if (x.find(key_name) != x.end()) {
            return mgr->assign(name, x.at(key_name));
          } else {
            return mgr->zero();
          }
        });
      } else if (value_type == "ignore_field") {
        funcs.push_back([mgr, key_name](const input_t &x) {
          if (x.find(key_name) != x.end()) {
            return mgr->one();
          } else {
            return mgr->zero();
          }
        });
      } else {
      }
    }
  }

  explicit record(const kwargs &kw)
      : record(reelay::any_cast<data_mgr_t>(kw.at("manager")),
               reelay::any_cast <
                   std::vector<std::pair<std::string,
                                         std::pair<std::string, std::string>>>>(
                       kw.at("fields"))) {}

  void update(const input_t &args) override {
    value = funcs[0](args);
    for (std::size_t i = 1; i < funcs.size(); i++) {
      value *= funcs[i](args);
    }
  }
  output_t output() override { return value; }
};

} // namespace untimed_data_setting
} // namespace reelay
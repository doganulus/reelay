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
      if (fields[i].first == "proposition") {
        auto name = fields[i].second;
        funcs.push_back([name, mgr](const input_t &x) {
          if (std::find(x.begin(), x.end(), name) != x.end())
            return mgr->one();
          else
            return mgr->zero();
        });
      } else if (fields[i].first == "variable_ref") {
        auto name = fields[i].second;
        mgr->add_variable(name);
        funcs.push_back([mgr, name, i](const input_t &x) {
          return mgr->assign(name, x[i]);
        });
      } else if (fields[i].first == "ignore_field") {
        funcs.push_back([mgr](const input_t &) { return mgr->one(); });
      } else {
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
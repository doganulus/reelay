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
struct proposition : public untimed_state<X, data_set_t> {
  explicit proposition(const kwargs &) {}
};

template <>
struct proposition<std::vector<std::string>>
    : public untimed_state<std::vector<std::string>, data_set_t> {
  using input_t = std::vector<std::string>;
  using output_t = data_set_t;

  using function_t = std::function<data_set_t(const input_t &)>;

  data_mgr_t manager;

  function_t fn;

  data_set_t value;

  explicit proposition(const data_mgr_t &mgr, const std::string &name)
      : manager(mgr), fn([name, mgr](const input_t &x) {
          if (std::find(x.begin(), x.end(), name) != x.end())
            return mgr->one();
          else
            return mgr->zero();
        })
  {
    value = manager->zero();
  }

  explicit proposition(const kwargs &kw)
      : proposition(reelay::any_cast<data_mgr_t>(kw.at("manager")),
                    reelay::any_cast<std::string>(kw.at("name"))) {}

  void update(const input_t &args) override { value = fn(args); }

  output_t output() override { return value; }
};

template <>
struct proposition<std::unordered_map<std::string, std::string>>
    : public untimed_state<std::unordered_map<std::string, std::string>,
                           data_set_t> {
  using input_t = std::unordered_map<std::string, std::string>;
  using output_t = data_set_t;

  using function_t = std::function<data_set_t(const input_t &)>;

  data_mgr_t manager;

  function_t fn;

  data_set_t value;

  explicit proposition(const data_mgr_t &mgr, const std::string &name)
      : manager(mgr), fn([name, mgr](const input_t &x) {
          if (x.find(name) != x.end()) {
            return mgr->one();
          } else {
            return mgr->zero();
          }
        }) {
    value = manager->zero();
  }

  explicit proposition(const kwargs &kw)
      : proposition(reelay::any_cast<data_mgr_t>(kw.at("manager")),
                    reelay::any_cast<std::string>(kw.at("name"))) {}

  void update(const input_t &args) override { value = fn(args); }

  output_t output() override { return value; }
};

} // namespace untimed_data_setting
} // namespace reelay
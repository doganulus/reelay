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

template <typename X, typename T>
struct proposition : public discrete_timed_state<X, data_set_t, T> {
  explicit proposition(const kwargs &) {}
};

template <typename T>
struct proposition<std::vector<std::string>, T>
    : public discrete_timed_state<std::vector<std::string>, data_set_t, T> {
  using time_t = T;
  using value_t = data_set_t;
  using output_t = data_set_t;
  using input_t = std::vector<std::string>;

  using function_t = std::function<output_t(const input_t &)>;

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

  void update(const input_t &args, time_t) override { value = fn(args); }

  output_t output(time_t) override { return value; }
};

template <typename T>
struct proposition<std::unordered_map<std::string, std::string>, T>
    : public discrete_timed_state<std::unordered_map<std::string, std::string>,
                                  data_set_t, T> {
  using time_t = T;
  using value_t = data_set_t;
  using output_t = data_set_t;
  using input_t = std::unordered_map<std::string, std::string>;

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

  void update(const input_t &args, time_t) override { value = fn(args); }

  output_t output(time_t) override { return value; }
};

} // namespace untimed_data_setting
} // namespace reelay
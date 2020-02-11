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
#include "reelay/past_temporal_logic/dense_timed_data/atoms.hpp"

namespace reelay {
namespace dense_timed_data_setting {

template <typename T>
struct listing<std::vector<std::string>, T, 1>
    : public dense_timed_state<std::vector<std::string>, data_interval_map<T>,
                               T> {

  using time_t = T;
  using input_t = std::vector<std::string>;
  using value_t = data_set_t;
  using output_t = reelay::data_interval_map<time_t>;

  using node_t = dense_timed_node<output_t, time_t>;
  using state_t = dense_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  using function_t = std::function<data_set_t(const input_t &)>;

  using interval = reelay::interval<time_t>;
  using interval_map = reelay::data_interval_map<time_t>;

  data_mgr_t manager;

  std::vector<function_t> funcs = {};

  interval_map value;

  explicit listing(
      const data_mgr_t &mgr,
      const std::vector<std::pair<std::string, std::string>> &fields)
      : manager(mgr) {
    for (std::size_t i = 0; i < fields.size(); i++) {
      if (fields[i].first == "proposition") {
        auto name = fields[i].second;
        funcs.push_back([mgr, name](const input_t &x) {
          if (std::find(x.begin(), x.end(), name) != x.end())
            return mgr->one();
          else
            return mgr->zero();
        });
      } else if (fields[i].first == "variable_ref") {
        auto name = fields[i].second;
        mgr->add_variable(name);
        funcs.push_back([mgr, name, i](const input_t &x) {
          return mgr->assign(name, x[i + 1]);
        });
      } else if (fields[i].first == "ignore_field") {
        funcs.push_back([mgr](const input_t &) { return mgr->one(); });
      } else {
      }
    }
  }

  explicit listing(const kwargs &kw)
      : listing(
            reelay::any_cast<data_mgr_t>(kw.at("manager")),
            reelay::any_cast<std::vector<std::pair<std::string, std::string>>>(
                kw.at("fields"))) {}

  void update(const input_t &pargs, const input_t &args, time_t previous,
              time_t now) override {
    data_set_t base_value;
    if (args.size() != funcs.size() + 1) {
      base_value = manager->zero();
    } else {
      base_value = manager->one();
      for (std::size_t i = 0; i < funcs.size(); i++) {
        base_value *= funcs[i](args);
      }
    }
    value = interval_map();
    value.add(std::make_pair(interval::open(previous, now), manager->zero()));
    value.add(std::make_pair(interval::closed(now, now), base_value));
  }

  output_t output(time_t, time_t) override { return value; }
};

} // namespace dense_timed_data_setting
} // namespace reelay
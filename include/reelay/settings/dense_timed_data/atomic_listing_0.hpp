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
#include "reelay/settings/dense_timed_data/atoms.hpp"

namespace reelay {
namespace dense_timed_data_setting {

template <typename T>
struct listing<std::vector<std::string>, T, 0>
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
      auto field_value_type = fields[i].first;
      auto field_value_str = fields[i].second;
      if (field_value_type == "bool" and field_value_str == "true") {
        funcs.push_back([mgr, i](const input_t &x) {
          if (field_of<input_t>::as_bool(x, i + 1)) {
            return mgr->one();
          } else {
            return mgr->zero();
          }
        });
      } else if (field_value_type == "bool" and field_value_str == "false") {
        funcs.push_back([mgr, i](const input_t &x) {
          if (field_of<input_t>::as_bool(x, i + 1)) {
            return mgr->zero();
          } else {
            return mgr->one();
          }
        });
      } else if (field_value_type == "string") {
        funcs.push_back([mgr, field_value_str, i](const input_t &x) {
          if (field_of<input_t>::as_string(x, i + 1) == field_value_str) {
            return mgr->one();
          } else {
            return mgr->zero();
          }
        });
      } else if (field_value_type == "number") {
        float number = boost::lexical_cast<float>(field_value_str);
        funcs.push_back([mgr, number, i](const input_t &x) {
          if (field_of<input_t>::as_float(x, i + 1) == number) {
            return mgr->one();
          } else {
            return mgr->zero();
          }
        });
      } else if (field_value_type == "variable_ref") {
        mgr->add_variable(field_value_str);
        funcs.push_back([mgr, field_value_str, i](const input_t &x) {
          return mgr->assign(field_value_str, x[i + 1]);
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
    value = interval_map(
        std::make_pair(interval::left_open(previous, now), base_value));
  }

  output_t output(time_t, time_t) override { return value; }
};

} // namespace dense_timed_data_setting
} // namespace reelay
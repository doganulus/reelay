/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>
#include <vector>

#include "reelay/common.hpp"
#include "reelay/networks/basic_structure.hpp"
#include "reelay/unordered_data.hpp"

namespace reelay {
namespace discrete_timed_data_setting {

template <typename X, typename T>
struct exists final : public discrete_timed_node<data_set_t, T> {
  using time_t = T;
  using input_t = X;
  using value_t = data_set_t;
  using output_t = data_set_t;

  using node_t = discrete_timed_node<output_t, time_t>;
  using state_t = discrete_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  node_ptr_t arg1;
  data_set_t cube;

  exists(const data_mgr_t &mgr, const std::vector<std::string> &vars,
                  const std::vector<node_ptr_t> &args)
      : arg1(args[0]) {
    cube = mgr->one();
    for (const auto &name : vars) {
      cube *= mgr->variables[name].cube;
    }
  }

  exists(const kwargs &kw)
      : exists(reelay::any_cast<data_mgr_t>(kw.at("manager")),
               reelay::any_cast<std::vector<std::string>>(kw.at("vars")),
               reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}

  output_t output(time_t now) override {
    data_set_t value = arg1->output(now);
    return value.ExistAbstract(cube);
  }
};

} // namespace untimed_data_setting
} // namespace reelay
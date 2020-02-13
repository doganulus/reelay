/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "vector"

#include "reelay/networks/basic_structure.hpp"

namespace reelay {
namespace untimed_data_setting {

template <typename X> struct forall : public untimed_node<data_set_t> {
  using input_t = X;
  using output_t = data_set_t;

  using node_t = untimed_node<output_t>;
  using node_ptr_t = std::shared_ptr<node_t>;

  node_ptr_t arg1;
  data_set_t cube;

  explicit forall(const data_mgr_t &mgr, const std::vector<std::string> &vars,
                  const std::vector<node_ptr_t> &args)
      : arg1(args[0]) {
    cube = mgr->one();
    for (const auto &name : vars) {
      cube *= mgr->variables[name].cube;
    }
  }

  explicit forall(const kwargs &kw)
      : forall(reelay::any_cast<data_mgr_t>(kw.at("manager")),
               reelay::any_cast<std::vector<std::string>>(kw.at("vars")),
               reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}

  output_t output() {
    data_set_t value = arg1->output();
    return value.UnivAbstract(cube);
  }
};

} // namespace untimed_data_setting
} // namespace reelay
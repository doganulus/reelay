/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "reelay/networks/basic_structure.hpp"

namespace reelay {

template <typename InputT, typename OutputT, typename TimeT = int>
struct untimed_network : untimed_state<InputT, OutputT> {
  using time_t = TimeT;
  using input_t = InputT;
  using output_t = OutputT;

  using node_t = untimed_node<output_t>;
  using state_t = untimed_state<input_t, output_t>;

  using type = untimed_network<input_t, output_t>;

  time_t _now = -1;  // For informative purposes

  std::shared_ptr<node_t> output_node;
  std::vector<std::shared_ptr<state_t>> states;

  untimed_network(std::shared_ptr<node_t> n,
                  const std::vector<std::shared_ptr<state_t>> &ss)
      : output_node(n), states(ss) {}

  void update(const input_t &args) {
    _now = _now + 1;
    for (const auto &state : this->states) {
      state->update(args);
    }
  }
  output_t output() { return this->output_node->output(); }

  time_t now() { return _now; }
};

} // namespace reelay
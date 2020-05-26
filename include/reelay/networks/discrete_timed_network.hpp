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

template <typename InputT, typename OutputT, typename TimeT>
struct discrete_timed_network : discrete_timed_state<InputT, OutputT, TimeT> {
  using time_t = TimeT;
  using input_t = InputT;
  using output_t = OutputT;

  using node_t = discrete_timed_node<output_t, time_t>;
  using state_t = discrete_timed_state<input_t, output_t, time_t>;

  using type = discrete_timed_network<input_t, output_t, time_t>;

  time_t _now = -1;

  std::shared_ptr<node_t> output_node;
  std::vector<std::shared_ptr<state_t>> states;

  discrete_timed_network(std::shared_ptr<node_t> n,
                         const std::vector<std::shared_ptr<state_t>> &ss)
      : output_node(n), states(ss) {}

  void update(const input_t &args) {
    this->_now = this->_now + time_t(1);
    this->update(args, this->_now);
  }

  void update(const input_t &args, time_t now) {
    for (const auto &state : this->states) {
      state->update(args, now);
    }
  }

  output_t output() { return this->output(this->_now); }
  
  output_t output(time_t now) { return this->output_node->output(now);}

  time_t now() { return _now; }
};

} // namespace reelay
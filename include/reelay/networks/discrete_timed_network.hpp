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

template <typename input_t, typename output_t, typename time_t>
struct discrete_timed_network
    : discrete_timed_state<input_t, output_t, time_t> {
  using time_type = time_t;
  using input_type = input_t;
  using output_type = output_t;

  using node_type = discrete_timed_node<output_t, time_t>;
  using state_type = discrete_timed_state<input_t, output_t, time_t>;

  using type = discrete_timed_network<input_t, output_t, time_t>;

  time_t _now = 0;

  std::shared_ptr<node_type> output_node;
  std::vector<std::shared_ptr<state_type>> states;

  discrete_timed_network(std::shared_ptr<node_type> n,
                         const std::vector<std::shared_ptr<state_type>> &ss)
      : output_node(n), states(ss) {}

  void update(const input_t &args) {
    this->_now = this->_now + 1;
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
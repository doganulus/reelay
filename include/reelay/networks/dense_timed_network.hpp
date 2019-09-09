/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "reelay/interval_set.hpp"
#include "reelay/lexical_cast.hpp"
#include "reelay/networks/basic_structure.hpp"

namespace reelay {

template <typename input_t, typename output_t, typename time_t>
struct dense_timed_network : dense_timed_state<input_t, output_t, time_t> {
  using time_type = time_t;
  using input_type = input_t;
  using output_type = output_t;
  using vector_output_type = std::vector<std::pair<time_t, bool>>;

  using type = dense_timed_network<input_t, output_t, time_t>;

  using node_type = dense_timed_node<output_t, time_t>;
  using state_type = dense_timed_state<input_t, output_t, time_t>;

  time_t previous = 0;
  time_t current = 0;

  input_t prevargs;

  std::shared_ptr<node_type> output_node;
  std::vector<std::shared_ptr<state_type>> states;

  dense_timed_network(std::shared_ptr<node_type> n,
                      std::vector<std::shared_ptr<state_type>> ss)
      : output_node(n), states(ss) {}

  void update(const input_t& args) {
    time_t now = boost::lexical_cast<time_t>(args.at("time"));
    this->previous = this->current;
    update(this->prevargs, args, this->previous, now);
    this->current = now;
    this->prevargs = args;
  }

  void update(const input_t& args, time_t now) {
    this->previous = this->current;
    update(this->prevargs, args, this->previous, now);
    this->current = now;
    this->prevargs = args;
  }

  void update(const input_t& pargs,
              const input_t& args,
              time_t previous,
              time_t now) noexcept override {
    for (const auto& state : this->states) {
      state->update(pargs, args, previous, now);
    }
  }

  output_t output() {
    return this->output_node->output(this->previous, this->current);
  }

  std::string output_verbosity_0() {
    output_t result = this->output_node->output(this->previous, this->current);

    std::ostringstream buffer;
    for (const auto& intv : result) {
      buffer << "Fails between times " << intv.lower() << " and "
             << intv.upper() << std::endl;
    }

    return buffer.str();
  }

  std::string output_verbosity_1() {
    output_t result = this->output_node->output(this->previous, this->current);
    std::ostringstream buffer;

    time_t t;

    if (result.empty()) {
      buffer << this->current << ","
             << "0" << std::endl;
    }

    for (const auto& intv : result) {
      if (intv.lower() > this->previous) {
        t = intv.upper();
        buffer << intv.lower() << ","
               << "0" << std::endl;
        buffer << intv.upper() << ","
               << "1" << std::endl;
      } else if (intv.upper() < this->current) {
        t = intv.upper();
        buffer << intv.upper() << ","
               << "1" << std::endl;
      }
    }

    if (t < this->current) {
      buffer << this->current << ","
             << "0" << std::endl;
    }

    return buffer.str();
  }

  std::string output_verbosity_2() {
    output_t result = this->output_node->output(this->previous, this->current);
    std::ostringstream buffer;

    if (result.empty()) {
      buffer << this->current << ",";
      for (auto const& [k, v] : this->prevargs) {
        if (k != "time") {
          buffer << v << ",";
        }
      }
      buffer << "0" << std::endl;
    }

    for (const auto& intv : result) {
      if (intv.lower() > this->previous) {
        buffer << intv.lower() << ",";
        for (auto const& [k, v] : this->prevargs) {
          if (k != "time") {
            buffer << v << ",";
          }
        }
        buffer << "0" << std::endl;

        buffer << intv.upper() << ",";
        for (auto const& [k, v] : this->prevargs) {
          if (k != "time") {
            buffer << v << ",";
          }
        }
        buffer << "1" << std::endl;

      } else {
        buffer << intv.upper() << ",";
        for (auto const& [k, v] : this->prevargs) {
          if (k != "time") {
            buffer << v << ",";
          }
        }
        buffer << "1" << std::endl;
      }
    }

    if (result.end()->upper() < this->current) {
      buffer << this->current << ",";
      for (auto const& [k, v] : this->prevargs) {
        if (k != "time") {
          buffer << v << ",";
        }
      }
      buffer << "0" << std::endl;
    }

    return buffer.str();
  }
};

}  // namespace reelay
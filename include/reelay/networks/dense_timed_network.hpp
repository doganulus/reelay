/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "reelay/intervals.hpp"
#include "reelay/lexical_cast.hpp"
#include "reelay/networks/basic_structure.hpp"

namespace reelay {

template <typename X, typename Y, typename T, typename V>
struct dense_timed_network : dense_timed_state<X, Y, T> {
  using time_t = T;
  using value_t = V;
  using input_t = X;
  using output_t = Y;

  using type = dense_timed_network<input_t, output_t, time_t, value_t>;

  using node_t = dense_timed_node<output_t, time_t>;
  using state_t = dense_timed_state<input_t, output_t, time_t>;

  time_t previous = 0;
  time_t current = 0;

  input_t prevargs;

  std::shared_ptr<node_t> output_node;
  std::vector<std::shared_ptr<state_t>> states;

  dense_timed_network(std::shared_ptr<node_t> n,
                      const std::vector<std::shared_ptr<state_t>> &ss)
      : output_node(n), states(ss) {}

  void setup(const input_t &prevargs) {
    this->previous = boost::lexical_cast<time_t>(prevargs.at("time"));
    this->prevargs = prevargs;
  }

  void setup(const input_t &prevargs, time_t previous) {
    this->previous = previous;
    this->prevargs = prevargs;
  }

  void update(const input_t &args) {
    time_t now = boost::lexical_cast<time_t>(args.at("time"));
    this->previous = this->current;
    update(this->prevargs, args, this->previous, now);
    this->current = now;
    this->prevargs = args;
  }

  void update(const input_t &args, time_t now) {
    this->previous = this->current;
    update(this->prevargs, args, this->previous, now);
    this->current = now;
    this->prevargs = args;
  }

  void update(const input_t &pargs, const input_t &args, time_t pre,
              time_t now) {
    for (const auto &state : this->states) {
      state->update(pargs, args, pre, now);
    }
  }

  output_t output() {
    return this->output(this->previous, this->current);
  }

  output_t output(time_t pre, time_t now) {
    return this->output_node->output(pre, now);
  }

  output_t noutput() {
    return interval_set<time_t>(
               interval<time_t>::left_open(this->previous, this->current)) -
           this->output();
  }

  std::vector<std::pair<time_t, V>> voutput() {
    time_t t;
    reelay::interval_set<time_t> result = this->output();
    auto vresult = std::vector<std::pair<time_t, V>>();

    if (result.empty()) {
      vresult.push_back(std::pair<time_t, V>(this->current, 0));
    } else {
      for (const auto &intv : result) {
        t = intv.upper();
        if (intv.lower() > this->previous) {
          vresult.push_back(std::pair<time_t, V>(intv.lower(), 0));
          vresult.push_back(std::pair<time_t, V>(intv.upper(), 1));
        } else {
          vresult.push_back(std::pair<time_t, V>(intv.upper(), 1));
        }
      }

      if (t < this->current) {
        vresult.push_back(std::pair<time_t, V>(this->current, 0));
      }
    }
    return vresult;
  }

  std::vector<std::pair<time_t, V>> nvoutput() {
    time_t t;
    reelay::interval_set<time_t> result = this->noutput();
    auto vresult = std::vector<std::pair<time_t, V>>();

    if (result.empty()) {
      vresult.push_back(std::pair<time_t, V>(this->current, 0));
    } else {
      for (const auto &intv : result) {
        t = intv.upper();
        if (intv.lower() > this->previous) {
          vresult.push_back(std::pair<time_t, V>(intv.lower(), 0));
          vresult.push_back(std::pair<time_t, V>(intv.upper(), 1));
        } else {
          vresult.push_back(std::pair<time_t, V>(intv.upper(), 1));
        }
      }

      if (t < this->current) {
        vresult.push_back(std::pair<time_t, V>(this->current, 0));
      }
    }
    return vresult;
  }

  std::pair<time_t, time_t> now() {
    return std::pair<time_t, time_t>(previous, current);
  }
};

} // namespace reelay
/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "memory"
#include "vector"

namespace reelay {

template <typename output_t>
struct untimed_node {
  virtual ~untimed_node() {}
  virtual output_t output() {
    throw std::runtime_error("Undefined output function");
  };
};

template <typename output_t, typename time_t>
struct discrete_timed_node {
  virtual ~discrete_timed_node() {}
  virtual output_t output(time_t) {
    throw std::runtime_error("Undefined output function");
  }
};

template <typename output_t, typename time_t>
struct dense_timed_node {
  virtual ~dense_timed_node() {}
  virtual output_t output(time_t, time_t) {
    throw std::runtime_error("Undefined output function");
  }
};

template <typename input_t, typename output_t>
struct untimed_state : untimed_node<output_t> {
  virtual ~untimed_state() {}
  virtual output_t output() override {
    throw std::runtime_error("Undefined output function");
  }

  virtual void update(const input_t&) {
    throw std::runtime_error("Undefined update function");
  }
};

template <typename input_t, typename output_t, typename time_t>
struct discrete_timed_state : discrete_timed_node<output_t, time_t> {
  virtual ~discrete_timed_state() {}
  virtual output_t output(time_t) override {
    throw std::runtime_error("Undefined output function");
  }

  virtual void update(const input_t&, time_t) {
    throw std::runtime_error("Undefined update function");
  }
};

template <typename input_t, typename output_t, typename time_t>
struct dense_timed_state : dense_timed_node<output_t, time_t> {
  virtual ~dense_timed_state() {}
  virtual output_t output(time_t, time_t) override {
    throw std::runtime_error("Undefined output function");
  }

  virtual void update(const input_t&, const input_t&, time_t, time_t) {
    throw std::runtime_error("Undefined update function");
  }
};

}  // namespace reelay
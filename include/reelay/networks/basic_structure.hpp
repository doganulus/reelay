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

template <typename OutputT>
struct untimed_node {
  virtual ~untimed_node() {}
  virtual OutputT output() {
    throw std::runtime_error("Undefined output function");
  };
};

template <typename OutputT, typename TimeT>
struct discrete_timed_node {
  virtual ~discrete_timed_node() {}
  virtual OutputT output(TimeT) {
    throw std::runtime_error("Undefined output function");
  }
};

template <typename OutputT, typename TimeT>
struct dense_timed_node {
  virtual ~dense_timed_node() {}
  virtual OutputT output(TimeT, TimeT) {
    throw std::runtime_error("Undefined output function");
  }
};

template <typename InputT, typename OutputT>
struct untimed_state : untimed_node<OutputT> {
  virtual ~untimed_state() {}
  virtual OutputT output() override {
    throw std::runtime_error("Undefined output function");
  }

  virtual void update(const InputT&) {
    throw std::runtime_error("Undefined update function");
  }
};

template <typename InputT, typename OutputT, typename TimeT>
struct discrete_timed_state : discrete_timed_node<OutputT, TimeT> {
  virtual ~discrete_timed_state() {}
  virtual OutputT output(TimeT) override {
    throw std::runtime_error("Undefined output function");
  }

  virtual void update(const InputT&, TimeT) {
    throw std::runtime_error("Undefined update function");
  }
};

template <typename InputT, typename OutputT, typename TimeT>
struct dense_timed_state : dense_timed_node<OutputT, TimeT> {
  virtual ~dense_timed_state() {}
  virtual OutputT output(TimeT, TimeT) override {
    throw std::runtime_error("Undefined output function");
  }

  virtual void update(const InputT&, const InputT&, TimeT, TimeT) {
    throw std::runtime_error("Undefined update function");
  }
};

}  // namespace reelay
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

template <typename OutputT, typename TimeT>
struct discrete_timed_node {
  virtual ~discrete_timed_node() {}
  virtual OutputT output(TimeT) = 0;
};

template <typename OutputT, typename TimeT>
struct dense_timed_node {
  virtual ~dense_timed_node() {}
  virtual OutputT output(TimeT, TimeT) = 0;
};

template <typename InputT, typename OutputT, typename TimeT>
struct discrete_timed_state : discrete_timed_node<OutputT, TimeT> {
  virtual ~discrete_timed_state() {}
  virtual OutputT output(TimeT) override = 0;
  virtual void update(const InputT&, TimeT) = 0;
};

template <typename InputT, typename OutputT, typename TimeT>
struct dense_timed_state : dense_timed_node<OutputT, TimeT> {
  virtual ~dense_timed_state() {}
  virtual OutputT output(TimeT, TimeT) override = 0;
  virtual void update(const InputT&, const InputT&, TimeT, TimeT) = 0;
};

}  // namespace reelay
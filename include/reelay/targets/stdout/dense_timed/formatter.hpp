/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */


#pragma once

#include "reelay/settings.hpp"

namespace reelay {
namespace dense_timed_setting {

template <typename X, typename T>
struct stdout_formatter {
  using setting = factory<X, T>;

  using input_t = typename setting::input_t;
  using output_t = typename setting::output_t;
  using function_t = typename setting::function_t;

  using network_t = typename setting::network_t;
  using network_ptr_t = typename setting::network_ptr_t;

  network_ptr_t network;
  std::vector<std::string> columms;

  void update(const input_t& args) { network->update(args); }
  void update(const input_t& args, time_t now) { network->update(args, now); }
  void update(const input_t& pargs,
              const input_t& args,
              time_t previous,
              time_t now) {
    network->update(pargs, args, previous, now);
  }

  virtual std::string header();
  virtual std::string output();

};

} // namespace dense_timed_setting
} // namespace reelay
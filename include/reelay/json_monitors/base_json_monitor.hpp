/*
 *                 |
 *    __| _ \  _ \ |  _` | |   |
 *   |    __/  __/ | (   | |   |
 *  _|  \___|\___|_|\__,_|\__, |
 *                         ____/    Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "string"

#include "reelay/json.hpp"

namespace reelay {

template <typename T> 
struct base_json_monitor {
  using time_t = T;
  using input_t = json;
  using output_t = json;

  const std::string t_name;
  const std::string y_name;
  const std::string pattern;

  virtual ~base_json_monitor(){};

  virtual output_t update(const input_t &args) = 0;
  virtual output_t update(const input_t &args, time_t now) = 0;

  virtual time_t now() = 0;
};

} // namespace reelay
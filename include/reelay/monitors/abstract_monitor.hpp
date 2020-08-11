/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "reelay/json.hpp"

namespace reelay {

template <typename InputT, typename OutputT>
struct abstract_monitor {
  using input_type = InputT;
  using output_type = OutputT;

  virtual ~abstract_monitor() {}
  virtual output_type update(const input_type& obj) = 0;
  // virtual json get(const std::string& attribute) = 0;
};

}
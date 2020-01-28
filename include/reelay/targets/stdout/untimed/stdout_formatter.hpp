/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "reelay/settings.hpp"

namespace reelay {
namespace untimed_setting {

template <typename X>
struct stdout_formatter : public untimed_state<X, std::string> {

  virtual std::string header();
  virtual std::string output();
};
} // namespace untimed_setting
} // namespace reelay
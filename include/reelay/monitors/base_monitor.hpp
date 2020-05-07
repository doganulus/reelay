/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

namespace reelay {

template <typename TimeT, typename InputT, typename OutputT>
struct base_monitor {
  using time_t = TimeT;
  using input_t = InputT;
  using output_t = OutputT;

  virtual output_t update(const input_t&) = 0;
  virtual time_t now() = 0;

};
}  // namespace reelay
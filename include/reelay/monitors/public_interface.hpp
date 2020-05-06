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

#include "reelay/constants.hpp"

namespace reelay {

template <typename TimeT>
struct discrete_timed {
  
  struct json_monitor;

  template <typename ValueT>
  struct robustness {

    struct json_monitor;

  };
};

template <typename TimeT>
struct condensing {
  struct json_monitor;

  template <typename ValueT>
  struct robustness {

    struct json_monitor;

  };
};

template <typename TimeT, piecewise InterpolationT = piecewise::CONSTANT>
struct dense_timed {
  struct json_monitor;

  template <typename ValueT>
  struct robustness {
    struct json_monitor;
  };
};

}  // namespace reelay
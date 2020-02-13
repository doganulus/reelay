/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "string"

namespace reelay {
namespace dense_timed_data_setting {

template <typename X, typename T, int type>
struct record : public dense_timed_state<X, data_interval_map<T>, T> {
  explicit record(const kwargs &) {}
};

template <typename X, typename T, int type>
struct listing : public dense_timed_state<X, data_interval_map<T>, T> {
  explicit listing(const kwargs &) {}
};

} // namespace dense_timed_data_setting
} // namespace reelay
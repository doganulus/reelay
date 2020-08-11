/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

namespace reelay {

template <typename TimeT, typename ValueT, typename OutputT, bool condensing>
struct discrete_timed_formatter {};

template <typename TimeT, typename ValueT, typename OutputT>
struct dense_timed_formatter {};

template <typename TimeT, typename ValueT, typename OutputT>
struct dense_timed_robustness_formatter {};

template <typename TimeT, typename ValueT, typename OutputT>
struct dense_timed_data_formatter {};

}  // namespace reelay
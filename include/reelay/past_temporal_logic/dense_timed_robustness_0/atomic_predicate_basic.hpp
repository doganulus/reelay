/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "functional"
#include "memory"
#include "vector"

namespace reelay {
namespace dense_timed_robustness_0_setting {

template <typename X, typename V, typename T> struct basic_predicate_lt {};
template <typename X, typename V, typename T> struct basic_predicate_le {};
template <typename X, typename V, typename T> struct basic_predicate_gt {};
template <typename X, typename V, typename T> struct basic_predicate_ge {};

}  // namespace dense_timed_setting
}  // namespace reelay
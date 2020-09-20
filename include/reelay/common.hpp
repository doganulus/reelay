/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>
#include <unordered_map>

#define PEGLIB_USE_STD_ANY 0
#include "reelay/third_party/cpp-peglib/peglib.h"

namespace reelay {

using any = peg::any;

template <typename T, typename... Args>
inline auto any_cast(Args &&... args)
    -> decltype(peg::any_cast<T>(std::forward<Args>(args)...)) {
  return peg::any_cast<T>(std::forward<Args>(args)...);
}

using kwargs = std::unordered_map<std::string, any>;

}  // namespace reelay

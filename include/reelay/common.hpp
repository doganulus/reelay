/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <any>
#include <string>
#include <unordered_map>

namespace reelay {

using any = std::any;

// template<typename T>
// using any_cast = std::any_cast; 

using kwargs = std::unordered_map<std::string, any>;

}  // namespace reelay
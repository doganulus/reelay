/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "vector"
#include "unordered_map"

#include "boost/lexical_cast.hpp"

namespace reelay {

// By default: Get timestamp using the keyword time
template <typename input_t, typename time_t> 
struct timestamp{
  inline static time_t from(const input_t &args) {
    return boost::lexical_cast<time_t>(args.at("time"));
  }
};

// Specialization: Get timestamp from the first element if a list
template <typename time_t>
struct timestamp<std::vector<std::string>, time_t> {
  inline static time_t from(const std::vector<std::string> &args) {
    return boost::lexical_cast<time_t>(args.at(0));
  }
};

} // namespace reelay
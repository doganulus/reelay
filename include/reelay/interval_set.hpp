/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "boost/icl/interval.hpp"
#include "boost/icl/interval_set.hpp"

namespace reelay {

template <class T>
using interval = boost::icl::interval<T>;

template <class T>
using interval_set = boost::icl::interval_set<T>;

}  // namespace reelay
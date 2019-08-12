/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "vector"
#include "reelay/networks/basic_structure.hpp"
#include "reelay/interval_set.hpp"

namespace reelay {
namespace discrete_timed_setting {

template<typename X, typename T>
struct since_bounded : public discrete_timed_state<X, bool, T>
{
	using time_t = T;
    using input_t = X;
    using output_t = bool;

    using node_t = discrete_timed_node<output_t, time_t>;
    using state_t = discrete_timed_state<input_t, output_t, time_t>;

    using node_ptr_t = std::shared_ptr<node_t>;
    using state_ptr_t = std::shared_ptr<state_t>;

    using interval = reelay::interval<time_t>;
    using interval_set = reelay::interval_set<time_t>;

    interval_set value = interval_set();

    node_ptr_t first;
    node_ptr_t second;

    time_t lbound = 0;
    time_t ubound = 0;

    since_bounded(std::vector<node_ptr_t> args, time_t l, time_t u) : first(args[0]), second(args[1]), lbound(l), ubound(u) {}

    void update(const input_t &args, time_t now)
    {
        if (first->output(now) and second->output(now))
        {
            value = value.add(interval::closed(now + lbound, now + ubound));
            value = value - interval_set(interval::right_open(0, now));
        }
        else if (not first->output(now) and second->output(now))
        {
            value = interval_set(interval::closed(now + lbound, now + ubound));
        }
        else if (first->output(now) and not second->output(now))
        {
            // Laziness -- No need to update
            // state = state - interval_set(interval::right_open(0, t.now));
        }
        else
        {
            value = interval_set();
        }
    }

    output_t output(time_t now)
    {
        return boost::icl::contains(value, now);
    }
};

} // namespace discrete_timed_setting
} // namespace reelay
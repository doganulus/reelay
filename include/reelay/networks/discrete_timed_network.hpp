/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "reelay/networks/basic_structure.hpp"

namespace reelay
{

template <
    typename input_t,
    typename output_t,
    typename time_t>
struct discrete_timed_network : discrete_timed_state<input_t, output_t, time_t>
{
    using time_type = time_t;
    using input_type = input_t;
    using output_type = output_t;
    
    using node_type = discrete_timed_node<output_t, time_t>;
    using state_type = discrete_timed_state<input_t, output_t, time_t>;

    using type = discrete_timed_network<input_t, output_t, time_t>;

    time_t _current_time = 0;

    std::shared_ptr<node_type> output_node;
    std::vector<std::shared_ptr<state_type>> states;

    discrete_timed_network(
        std::shared_ptr<node_type> n,
        std::vector<std::shared_ptr<state_type>> ss)
        : output_node(n), states(ss) {}

    inline void tick()
    {
        _current_time = _current_time + 1;
    }
    inline void set_current_time(time_t now)
    {
        _current_time = now;
    }
    inline time_t get_current_time()
    {
        return _current_time;
    }

    void update(const input_t &args)
    {
        this->tick();
        for (const auto &state : this->states)
        {
            state->update(args, this->get_current_time());
        }
    }

    // void update(const input_t &args, time_t now) override
    // {
    //     this->set_current_time(now);
    //     for (const auto &state : this->states)
    //     {
    //         state->update(args, this->get_current_time());
    //     }
    // }

    output_t output()
    {
        return this->output_node->output(this->get_current_time());
    }
};

} //namespace reelay
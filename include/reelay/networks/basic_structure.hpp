/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "vector"
#include "memory"

namespace reelay {

template <typename output_t>
struct untimed_node
{
    virtual output_t output(){
        throw std::runtime_error("Undefined output function");
    };
};

template <typename output_t, typename time_t>
struct discrete_timed_node
{
    virtual output_t output(time_t now)
    {
        throw std::runtime_error("Undefined output function");
    }
};

template <typename output_t, typename time_t>
struct dense_timed_node
{
    virtual output_t output(time_t previous, time_t now)
    {
        throw std::runtime_error("Undefined output function");
    }
};

template <typename input_t, typename output_t>
struct untimed_state : untimed_node<output_t>
{
    virtual output_t output() override
    {
        throw std::runtime_error("Undefined output function");
    }

    virtual void update(const input_t &args)
    {
        throw std::runtime_error("Undefined update function");
    }
    
};

template <typename input_t, typename output_t, typename time_t>
struct discrete_timed_state : discrete_timed_node<output_t, time_t>
{
    virtual output_t output(time_t now) override
    {
        throw std::runtime_error("Undefined output function");
    }

    virtual void update(const input_t &args, time_t now)
    {
        throw std::runtime_error("Undefined update function");
    }
    
};

template <typename input_t, typename output_t, typename time_t>
struct dense_timed_state : dense_timed_node<output_t, time_t>
{
    virtual output_t output(time_t previous, time_t now) override
    {
        throw std::runtime_error("Undefined output function");
    }

    virtual void update(const input_t &pargs, const input_t &args, time_t previous, time_t now)
    {
        throw std::runtime_error("Undefined update function");
    }
    
};

} // namespace reelay
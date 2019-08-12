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
#include "functional"

#include "reelay/networks/basic_structure.hpp"
#include "reelay/networks/discrete_timed_network.hpp"

namespace reelay {
namespace discrete_timed_setting {

template<typename X, typename T>
struct proposition : public discrete_timed_state<X, bool, T>
{
    using time_t = T;
    using input_t = X;
    using output_t = bool;

    using function_t = std::function<bool(const input_t &)>;

    bool value = false;
    function_t fn;

    proposition(std::string name) 
        : fn( [name](const input_t &x) { 
            return std::stoi(x.at(name));
        }) {}

    void update(const input_t &args, time_t now) override
    {
        value = fn(args);
    }

    output_t output(time_t now) override
    {
        return value;
    }
};

template<typename X, typename T>
struct basic_predicate_lt : public discrete_timed_state<X, bool, T>
{
    using time_t = T;
    using input_t = X;
    using output_t = bool;

    using function_t = std::function<bool(const input_t &)>;

    bool value = false;
    function_t fn;

    basic_predicate_lt(std::string name, float c)
        : fn( [name,c](const input_t &x) { return std::stof(x.at(name)) < c ;}) {}

    void update(const input_t &args, time_t now) override
    {
        value = fn(args);
    }

    output_t output(time_t now) override
    {
        return value;
    }
};

template<typename X, typename T>
struct basic_predicate_le : public discrete_timed_state<X, bool, T>
{
    using time_t = T;
    using input_t = X;
    using output_t = bool;

    using function_t = std::function<bool(const input_t &)>;

    bool value = false;
    function_t fn;

    basic_predicate_le(std::string name, float c)
        : fn( [name,c](const input_t &x) { return std::stof(x.at(name)) <= c ;}) {}

    void update(const input_t &args, time_t now) override
    {
        value = fn(args);
    }

    output_t output(time_t now) override
    {
        return value;
    }
};

template<typename X, typename T>
struct basic_predicate_gt : public discrete_timed_state<X, bool, T>
{
    using time_t = T;
    using input_t = X;
    using output_t = bool;

    using function_t = std::function<bool(const input_t &)>;

    bool value = false;
    function_t fn;

    basic_predicate_gt(std::string name, float c)
        : fn( [name,c](const input_t &x) { return std::stof(x.at(name)) > c ;}) {}

    void update(const input_t &args, time_t now) override
    {
        value = fn(args);
    }

    output_t output(time_t now) override
    {
        return value;
    }
};

template<typename X, typename T>
struct basic_predicate_ge : public discrete_timed_state<X, bool, T>
{
    using time_t = T;
    using input_t = X;
    using output_t = bool;

    using function_t = std::function<bool(const input_t &)>;

    bool value = false;
    function_t fn;

    basic_predicate_ge(std::string name, float c)
        : fn( [name,c](const input_t &x) { return std::stof(x.at(name)) >= c ;}) {}

    void update(const input_t &args, time_t now) override
    {
        value = fn(args);
    }

    output_t output(time_t now) override
    {
        return value;
    }
};

template<typename X, typename T>
struct predicate : public discrete_timed_state<X, bool, T>
{
    using time_t = T;
    using input_t = X;
    using output_t = bool;

    using function_t = std::function<bool(const input_t &)>;

    bool value = false;
    function_t fn;

    predicate(const function_t &f) : fn(f) {}

    void update(const input_t &args, time_t now) override
    {
        value = fn(args);
    }

    output_t output(time_t now) override
    {
        return value;
    }
};

} // namespace discrete_timed_setting
} // namespace reelay 
/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "map"
#include "string"
#include "memory"
#include "functional"

#include "reelay/networks.hpp"
#include "reelay/settings.hpp"
#include "reelay/builders.hpp"

#include "reelay/intervals.hpp"
#include "reelay/verbosity.hpp"

namespace reelay {

template <typename input_t>
struct monitor {
    using factory = untimed_setting::factory<input_t>;

    using network_t = typename factory::network_t;
    using network_ptr_t = typename factory::network_ptr_t;

    using function_t = typename factory::function_t;

    static network_ptr_t from_temporal_logic(
        std::string pattern,
        std::map<std::string, function_t> predicates =
            std::map<std::string, function_t>()
    ) {
        return make_network<factory>::from_temporal_logic(pattern, predicates);
    }
//    static type from_regular_expressions();
};

// template <typename value_t>
// struct robustness {
//     template <typename input_t>
//     struct monitor
//     {
//     };
// };

template <typename time_t>
struct discrete_timed {
    template <typename input_t>
    struct monitor {
        using factory = discrete_timed_setting::factory<input_t, time_t>;

        using network_t = typename factory::network_t;
        using network_ptr_t = typename factory::network_ptr_t;

        using function_t = typename factory::function_t;

        static network_ptr_t from_temporal_logic(
            std::string pattern,
            std::map<std::string, function_t> predicates =
                std::map<std::string, function_t>()
        ) {
            return make_network<factory>::from_temporal_logic(
                pattern, predicates);
        }
        // static network_ptr_t from_regular_expressions();
    };

    // template <typename value_t>
    // struct robustness {
    //     template <typename input_t>
    //     struct monitor
    //     {
    //     };
    // };
};

template <typename time_t>
struct dense_timed {
    template <typename input_t>
    struct monitor {
        using factory = dense_timed_setting::factory<input_t, time_t>;

        using network_t = typename factory::network_t;
        using network_ptr_t = typename factory::network_ptr_t;

        using function_t = typename factory::function_t;

        using formatter_type = std::shared_ptr<
            DenseTimedStringFormatter<input_t, time_t>>;

        static network_ptr_t from_temporal_logic(
            std::string pattern,
            std::map<std::string, function_t> predicates =
                std::map<std::string, function_t>()
        ) {
            return make_network<factory>::from_temporal_logic(
                pattern, predicates);
        }

        // static type from_regular_expressions();

        static formatter_type string_formatter(int verbosity) {
            if (verbosity >= 2) {
                return std::make_shared<
                    DenseTimedVerbosity2<input_t, time_t>>();
            } else if (verbosity == 1) {
                return std::make_shared<
                    DenseTimedVerbosity1<input_t, time_t>>();
            } else {
                return std::make_shared<
                    DenseTimedVerbosity0<input_t, time_t>>();
            }
        }
    };

    // template <typename value_t>
    // struct robustness {
    //     template <typename input_t>
    //     struct monitor
    //     {
    //     };
    // };
};
}  // namespace reelay

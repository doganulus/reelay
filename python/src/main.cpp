/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "pybind11/pybind11.h"

#include "reelay/networks.hpp"
#include "reelay/monitors.hpp"

namespace py = pybind11;

struct past_ltl_monitor {
    using input_t = std::map<std::string, int64_t>;
    using factory = reelay::untimed_setting::factory<input_t>;

    using network_t = typename factory::network_t;
    using network_ptr_t = typename factory::network_ptr_t;

    static network_ptr_t from(const std::string& pattern){
        auto parser = ptl_parser<factory>(predicates);
        return parser.parse(pattern);
    }
};

PYBIND11_MODULE(reelay, m) {
    m.doc() = "Runtime verification using formal specifications"; // optional module docstring

    using mtlmon_t = reelay::discrete_timed_network<
        std::map<std::string, int64_t>,
        std::map<std::string, int64_t>
        >;

    using stlmon_t = reelay::dense_timed_network<
        std::map<std::string, double>,
        std::vector<std::map<std::string, int64_t>>
        >;

    py::class_<ltlmon_t>(m, "ltlmon")
        .def(py::init<const std::string &>())
        .def("setName", &Pet::setName)
        .def("getName", &Pet::getName);

}

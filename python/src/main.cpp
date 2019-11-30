/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include "reelay/recipes.hpp"

PYBIND11_MODULE(recipes, m) {
    m.doc() = "This package provides Python bindings of runtime monitors constructed from formal specifications using Reelay library."; // optional module docstring

    pybind11::class_<reelay::past_ltl_monitor>(m, "past_ltl_monitor")
        .def(pybind11::init<const std::string &>())
        .def("update", &reelay::past_ltl_monitor::update);

    pybind11::class_<reelay::discrete_timed_past_mtl_monitor>(
        m, "discrete_timed_past_mtl_monitor")
        .def(pybind11::init<const std::string &>())
        .def("update", &reelay::discrete_timed_past_mtl_monitor::update);

    pybind11::class_<reelay::dense_timed_past_mtl_monitor>(
        m, "dense_timed_past_mtl_monitor")
        .def(pybind11::init<const std::string &>())
        .def("update", &reelay::dense_timed_past_mtl_monitor::update);

    pybind11::class_<reelay::untimed_past_stl_monitor>(
        m, "untimed_past_stl_monitor")
        .def(pybind11::init<const std::string &>())
        .def("update", &reelay::untimed_past_stl_monitor::update);

    pybind11::class_<reelay::discrete_timed_past_stl_monitor>(
        m, "discrete_timed_past_stl_monitor")
        .def(pybind11::init<const std::string &>())
        .def("update", &reelay::discrete_timed_past_stl_monitor::update);

    pybind11::class_<reelay::dense_timed_past_stl0_monitor>(
        m, "dense_timed_past_stl0_monitor")
        .def(pybind11::init<const std::string &>())
        .def("update", &reelay::dense_timed_past_stl0_monitor::update);

    pybind11::class_<reelay::dense_timed_past_stl1_monitor>(
        m, "dense_timed_past_stl1_monitor")
        .def(pybind11::init<const std::string &>())
        .def("update", &reelay::dense_timed_past_stl1_monitor::update)
        .def("init_update",
             &reelay::dense_timed_past_stl1_monitor::init_update);

    pybind11::class_<reelay::untimed_past_rstl_monitor>(
        m, "untimed_past_rstl_monitor")
        .def(pybind11::init<const std::string &>())
        .def("update", &reelay::untimed_past_rstl_monitor::update);

    pybind11::class_<reelay::discrete_timed_past_rstl_monitor>(
        m, "discrete_timed_past_rstl_monitor")
        .def(pybind11::init<const std::string &>())
        .def("update", &reelay::discrete_timed_past_rstl_monitor::update);
}

/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
//
#include "reelay/monitors/monitor.hpp"
#include "reelay/parser/ptl_inspector.hpp"
//
#include "reelay/monitors/dense_timed_data_monitor.hpp"
#include "reelay/monitors/dense_timed_monitor.hpp"
#include "reelay/monitors/dense_timed_robustness_0_monitor.hpp"
#include "reelay/monitors/discrete_timed_data_monitor.hpp"
#include "reelay/monitors/discrete_timed_monitor.hpp"
#include "reelay/monitors/discrete_timed_robustness_monitor.hpp"
//
#include "reelay/formatters/python_formatter.hpp"
//
#include "reelay/options.hpp"

namespace py = pybind11;
namespace ry = reelay;

py::dict inspect(const std::string &pattern) {
  auto gadget = reelay::ptl_inspector();
  auto result = gadget.inspect(pattern);

  bool timed = reelay::any_cast<bool>(result["timed"]);
  bool has_references = reelay::any_cast<bool>(result["has_references"]);

  return py::dict(py::arg("timed") = timed,
                  py::arg("has_references") = has_references);
}

PYBIND11_MODULE(library, m) {
  m.doc()
      = "This package provides Python bindings of runtime monitors constructed "
        "from formal specifications using Reelay C++ library.";

  m.def("inspect", &inspect, "A function to inspect specifications");

  py::class_<ry::basic_options>(m, "monitor_options")
      .def(py::init<const std::string &, const std::string &, bool, bool>())
      .def(py::init<
           const std::string &, const std::string &, const std::string &,
           bool>());
  ;

  using discrete_monitor_t
      = ry::discrete_timed_monitor<intmax_t, py::object, py::object, false>;
  py::class_<discrete_monitor_t>(m, "discrete_monitor")
      .def("make", &discrete_monitor_t::make)
      .def("now", &discrete_monitor_t::now)
      .def("update", &discrete_monitor_t::update);

  using condensing_monitor_t
      = ry::discrete_timed_monitor<intmax_t, py::object, py::object, true>;
  py::class_<condensing_monitor_t>(m, "condensing_monitor")
      .def("make", &condensing_monitor_t::make)
      .def("now", &condensing_monitor_t::now)
      .def("update", &condensing_monitor_t::update);

  using discrete_data_monitor_t = ry::discrete_timed_data_monitor<
      intmax_t, py::object, py::object, false>;
  py::class_<discrete_data_monitor_t>(m, "discrete_data_monitor")
      .def("make", &discrete_data_monitor_t::make)
      .def("now", &discrete_data_monitor_t::now)
      .def("update", &discrete_data_monitor_t::update);

  using condensing_data_monitor_t
      = ry::discrete_timed_data_monitor<intmax_t, py::object, py::object, true>;
  py::class_<condensing_data_monitor_t>(m, "condensing_data_monitor")
      .def("make", &condensing_data_monitor_t::make)
      .def("now", &condensing_data_monitor_t::now)
      .def("update", &condensing_data_monitor_t::update);

  using discrete_robustness_monitor_t = ry::discrete_timed_robustness_monitor<
      intmax_t, double, py::object, py::object, false>;
  py::class_<discrete_robustness_monitor_t>(m, "discrete_robustness_monitor")
      .def("make", &discrete_robustness_monitor_t::make)
      .def("now", &discrete_robustness_monitor_t::now)
      .def("update", &discrete_robustness_monitor_t::update);

  using condensing_robustness_monitor_t = ry::discrete_timed_robustness_monitor<
      intmax_t, double, py::object, py::object, true>;
  py::class_<condensing_robustness_monitor_t>(
      m, "condensing_robustness_monitor")
      .def("make", &condensing_robustness_monitor_t::make)
      .def("now", &condensing_robustness_monitor_t::now)
      .def("update", &condensing_robustness_monitor_t::update);

  using dense_monitor_t = ry::dense_timed_monitor<
      double, py::object, py::object>;
  py::class_<dense_monitor_t>(m, "dense_monitor")
      .def("make", &dense_monitor_t::make)
      .def("now", &dense_monitor_t::now)
      .def("update", &dense_monitor_t::update);

  using dense_data_monitor_t = ry::dense_timed_data_monitor<
      double, py::object, py::object>;
  py::class_<dense_data_monitor_t>(m, "dense_data_monitor")
      .def("make", &dense_data_monitor_t::make)
      .def("now", &dense_data_monitor_t::now)
      .def("update", &dense_data_monitor_t::update);

  using dense_robustness_monitor_t
      = ry::dense_timed_robustness_0_monitor<double, double, py::object, py::object>;
  py::class_<dense_robustness_monitor_t>(m, "dense_robustness_monitor")
      .def("make", &dense_robustness_monitor_t::make)
      .def("now", &dense_robustness_monitor_t::now)
      .def("update", &dense_robustness_monitor_t::update);
}

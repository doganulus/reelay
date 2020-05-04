/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include "reelay/parser/ptl_inspector.hpp"

#include "recipes/monitor_discrete_categ.hpp"
#include "recipes/monitor_discrete_prop.hpp"
#include "recipes/monitor_discrete_robust.hpp"
#include "recipes/monitor_discrete_categ_condensing.hpp"
#include "recipes/monitor_discrete_prop_condensing.hpp"
#include "recipes/monitor_discrete_robust_condensing.hpp"

#include "recipes/monitor_dense_categ_const_float64.hpp"
#include "recipes/monitor_dense_categ_const_int64.hpp"
#include "recipes/monitor_dense_prop_const_float64.hpp"
#include "recipes/monitor_dense_prop_const_int64.hpp"
#include "recipes/monitor_dense_prop_linear_float64.hpp"
#include "recipes/monitor_dense_robust_const_float64.hpp"
#include "recipes/monitor_dense_robust_const_int64.hpp"

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
        "from formal specifications using Reelay library.";

  m.def("inspect", &inspect, "A function to inspect specifications");

  py::class_<ry::monitor_discrete_prop>(m, "monitor_discrete_prop")
      .def(py::init<const std::string &, const std::string &,
                    const std::string &>())
      .def("now", &ry::monitor_discrete_prop::now)
      .def("update", &ry::monitor_discrete_prop::update);

  py::class_<ry::monitor_discrete_categ>(m, "monitor_discrete_categ")
      .def(py::init<const std::string &, const std::string &,
                    const std::string &>())
      .def("now", &ry::monitor_discrete_categ::now)
      .def("update", &ry::monitor_discrete_categ::update);

  py::class_<ry::monitor_discrete_robust>(m, "monitor_discrete_robust")
      .def(py::init<const std::string &, const std::string &,
                    const std::string &>())
      .def("now", &ry::monitor_discrete_robust::now)
      .def("update", &ry::monitor_discrete_robust::update);

  py::class_<ry::monitor_discrete_prop_condensing>(
      m, "monitor_discrete_prop_condensing")
      .def(py::init<const std::string &, const std::string &,
                    const std::string &>())
      .def("now", &ry::monitor_discrete_prop_condensing::now)
      .def("update", &ry::monitor_discrete_prop_condensing::update);

  py::class_<ry::monitor_discrete_categ_condensing>(
      m, "monitor_discrete_categ_condensing")
      .def(py::init<const std::string &, const std::string &,
                    const std::string &>())
      .def("now", &ry::monitor_discrete_categ_condensing::now)
      .def("update", &ry::monitor_discrete_categ_condensing::update);

  py::class_<ry::monitor_discrete_robust_condensing>(
      m, "monitor_discrete_robust_condensing")
      .def(py::init<const std::string &, const std::string &,
                    const std::string &>())
      .def("now", &ry::monitor_discrete_robust_condensing::now)
      .def("update", &ry::monitor_discrete_robust_condensing::update);

  py::class_<ry::monitor_dense_prop_const_int64>(
      m, "monitor_dense_prop_const_int64")
      .def(py::init<const std::string &, const std::string &,
                    const std::string &>())
      .def("now", &ry::monitor_dense_prop_const_int64::now)
      .def("update", &ry::monitor_dense_prop_const_int64::update);

  py::class_<ry::monitor_dense_prop_const_float64>(
      m, "monitor_dense_prop_const_float64")
      .def(py::init<const std::string &, const std::string &,
                    const std::string &>())
      .def("now", &ry::monitor_dense_prop_const_float64::now)
      .def("update", &ry::monitor_dense_prop_const_float64::update);

  py::class_<ry::monitor_dense_prop_linear_float64>(
      m, "monitor_dense_prop_linear_float64")
      .def(py::init<const std::string &, const std::string &,
                    const std::string &>())
      .def("now", &ry::monitor_dense_prop_linear_float64::now)
      .def("update", &ry::monitor_dense_prop_linear_float64::update);

  py::class_<ry::monitor_dense_categ_const_int64>(
      m, "monitor_dense_categ_const_int64")
      .def(py::init<const std::string &, const std::string &,
                    const std::string &>())
      .def("now", &ry::monitor_dense_categ_const_int64::now)
      .def("update", &ry::monitor_dense_categ_const_int64::update);

  py::class_<ry::monitor_dense_categ_const_float64>(
      m, "monitor_dense_categ_const_float64")
      .def(py::init<const std::string &, const std::string &,
                    const std::string &>())
      .def("now", &ry::monitor_dense_categ_const_float64::now)
      .def("update", &ry::monitor_dense_categ_const_float64::update);

  py::class_<ry::monitor_dense_robust_const_int64>(
      m, "monitor_dense_robust_const_int64")
      .def(py::init<const std::string &, const std::string &,
                    const std::string &>())
      .def("now", &ry::monitor_dense_robust_const_int64::now)
      .def("update", &ry::monitor_dense_robust_const_int64::update);

  py::class_<ry::monitor_dense_robust_const_float64>(
      m, "monitor_dense_robust_const_float64")
      .def(py::init<const std::string &, const std::string &,
                    const std::string &>())
      .def("now", &ry::monitor_dense_robust_const_float64::now)
      .def("update", &ry::monitor_dense_robust_const_float64::update);
}

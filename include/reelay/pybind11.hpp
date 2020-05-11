/*
 *                 |             
 *    __| _ \  _ \ |  _` | |   | 
 *   |    __/  __/ | (   | |   | 
 *  _|  \___|\___|_|\__,_|\__, | 
 *                         ____/    Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "vector"
#include "unordered_set"

#include "pybind11/pybind11.h"

#include "reelay/datafield.hpp"

using namespace pybind11::literals;

namespace reelay {

using pyobj = pybind11::object;

template <typename T>
struct timefield<T, pybind11::object> {
  using input_t = pybind11::object;
  inline static T get_time(const input_t &container) {
    return container["time"].cast<T>();
  }
};

template <> struct datafield<pybind11::object> {
  using input_t = pybind11::object;
  static const std::unordered_set<std::string> falsity;

  inline static input_t at(const input_t &container, const std::string &key) {
    return container[key.c_str()];
  }

  inline static input_t at(const input_t &container, int index) {
    return container[pybind11::int_(index)];
  }

  inline static bool contains(const input_t &container, const std::string &key){
    return container.contains(key);
  }

  inline static bool as_bool(const input_t &container, const std::string &key) {
    return container[key.c_str()].cast<bool>();
  }

  inline static int as_integer(const input_t &container,
                                  const std::string &key) {
    return container[key.c_str()].cast<long>();
  }

  inline static double as_floating(const input_t &container,
                                  const std::string &key) {
    return container[key.c_str()].cast<double>();
  }

  inline static std::string as_string(const input_t &container,
                                      const std::string &key) {
    return container[key.c_str()].cast<std::string>();
  }

  inline static bool contains(const input_t &container, int index) {
    throw std::runtime_error("");
  }

  inline static bool as_bool(const input_t &container, int index) {
    throw std::runtime_error("");
  }

  inline static int as_integer(const input_t &container, int index) {
    throw std::runtime_error("");
  }

  inline static double as_floating(const input_t &container, int index) {
    throw std::runtime_error("");
  }

  inline static std::string as_string(const input_t &container, int index) {
    throw std::runtime_error("");
  }

  template <typename time_t>
  inline static time_t timestamp(const input_t &container) {
    return container["time"].cast<time_t>();
  }
};

const std::unordered_set<std::string> datafield<pyobj>::falsity = {"0", "false",
                                                                   "False"};

} // namespace reelay
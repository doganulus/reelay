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

#include "unordered_set"

#include "reelay/datafield.hpp"

namespace reelay {

template <typename T>
struct timefield<T, simdjson::dom::element> {
  using input_t = simdjson::dom::element;
  inline static T get_time(const input_t &container) {
    return container.at_key("time");
  }
};

template <>
struct datafield<simdjson::dom::element> {
  using input_t = simdjson::dom::element;

  inline static input_t at(const input_t &container, const std::string &key) {
    return container.at_key(key);
  }

  inline static input_t at(const input_t &container, std::size_t index) {
    return container.at(index);
  }

  inline static bool contains(const input_t &container, const std::string &key){
    for (simdjson::dom::object::iterator field
         = simdjson::dom::object(container).begin();
         field != simdjson::dom::object(container).end(); ++field) {
      if (key == field.key()) {
        return true;
      }
    }
    return false;
  }

  inline static bool as_bool(const input_t &container, const std::string &key) {
    return container.at_key(key).get<bool>();
  }

  inline static int64_t as_integer(const input_t &container,
                                  const std::string &key) {
    return container.at_key(key).get<int64_t>();
  }

  inline static double as_floating(const input_t &container,
                                  const std::string &key) {
    return container.at_key(key).get<double>();
  }

  inline static std::string as_string(const input_t &container,
                                           const std::string &key) {
    std::string_view sv = container.at_key(key).get<std::string_view>();
    return std::string(sv);
  }

  inline static bool contains(const input_t &container, std::size_t index) {
    return index < simdjson::dom::array(container).size();
  }

  inline static bool as_bool(const input_t &container, std::size_t index) {
    return container.at(index).get<bool>();
  }

  inline static int as_integer(const input_t &container, std::size_t index) {
    return container.at(index).get<int64_t>();
  }

  inline static double as_floating(const input_t &container,
                                   std::size_t index) {
    return container.at(index).get<double>();
  }

  inline static std::string as_string(const input_t &container,
                                      std::size_t index) {
    std::string_view sv = container.at(index).get<std::string_view>();
    return std::string(sv);
  }
};

} // namespace reelay
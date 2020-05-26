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
#include "reelay/third_party/nlohmann/json.hpp"

namespace reelay {

using json = nlohmann::json;

template <typename T>
struct timefield<T, json> {
  using input_t = json;
  inline static T get_time(const input_t &container) {
    return T(container.at("time"));
  }
};

template <> struct datafield<json> {
  using input_t = json;

  inline static input_t at(const input_t &container, const std::string &key) {
    return container[key];
  }

  inline static input_t at(const input_t &container, std::size_t index) {
    return container[index];
  }

  inline static bool contains(const input_t &container, const std::string &key){
    return container.find(key) != container.end();
  }

  inline static bool as_bool(const input_t &container, const std::string &key) {
    return container.at(key);
  }

  inline static int as_integer(const input_t &container,
                                  const std::string &key) {
    return container.at(key);
  }

  inline static double as_floating(const input_t &container,
                                  const std::string &key) {
    return container.at(key);
  }

  inline static std::string as_string(const input_t &container,
                                      const std::string &key) {
    return container.at(key);
  }

  inline static bool contains(const input_t &container, std::size_t index) {
    return index < container.size();
  }

  inline static bool as_bool(const input_t &container, std::size_t index) {
    return container.at(index);
  }

  inline static int as_integer(const input_t &container, std::size_t index) {
    return container.at(index);
  }

  inline static double as_floating(const input_t &container,
                                   std::size_t index) {
    return container.at(index);
  }

  inline static std::string as_string(const input_t &container,
                                      std::size_t index) {
    return container.at(index);
  }


};

} // namespace reelay
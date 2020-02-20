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

template <> struct datafield<json> {
  using input_t = json;
  static const std::unordered_set<std::string> falsity;

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

  template <typename time_t>
  inline static time_t timestamp(const input_t &container) {
    return container.at("time");
  }
};

const std::unordered_set<std::string>
    datafield<json>::falsity = {
        "0", "false", "False"};

} // namespace reelay
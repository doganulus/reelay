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

#include "algorithm"
#include "unordered_map"
#include "unordered_set"
#include "vector"


namespace reelay {

template <typename time_t, typename input_t>
struct timefield {};

template <typename input_t>
struct datafield {};

template <typename T>
struct timefield<T, std::unordered_map<std::string, std::string>> {
  using input_t = std::unordered_map<std::string, std::string>;
  inline static T get_time(const input_t &container) {
    return container.at("time");
  }
};

template <> struct datafield<std::unordered_map<std::string, std::string>> {
  using input_t = std::unordered_map<std::string, std::string>;
  static const std::unordered_set<std::string> falsity;

  inline static bool contains(const input_t &container, const std::string &key) {
    return container.find(key) != container.end();
  }

  inline static bool as_bool(const input_t &container, const std::string &key) {
    return falsity.find(container.at(key)) == falsity.end();
  }

  inline static int as_integer(const input_t &container,
                               const std::string &key) {
    return std::stoi(container.at(key));
  }

  inline static double as_floating(const input_t &container,
                                   const std::string &key) {
    return std::stod(container.at(key));
  }

  inline static std::string as_string(const input_t &container,
                                      const std::string &key) {
    return container.at(key);
  }

  inline static bool contains(const input_t &container, std::size_t index) {
    throw std::runtime_error("");
  }

  inline static bool as_bool(const input_t &container, std::size_t index) {
    throw std::runtime_error("");
  }

  inline static int as_integer(const input_t &container, std::size_t index) {
    throw std::runtime_error("");
  }

  inline static double as_floating(const input_t &container,
                                   std::size_t index) {
    throw std::runtime_error("");
  }

  inline static std::string as_string(const input_t &container,
                                      std::size_t index) {
    throw std::runtime_error("");
  }
};

const std::unordered_set<std::string>
    datafield<std::unordered_map<std::string, std::string>>::falsity = {
        "0", "false", "False"};

} // namespace reelay
/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "vector"
#include "unordered_map"
#include "unordered_set"

#include "boost/lexical_cast.hpp"

namespace reelay {

// By default: Get timestamp using the keyword time
template <typename input_t, typename time_t> 
struct timestamp{
  inline static time_t from(const input_t &args) {
    return boost::lexical_cast<time_t>(args.at("time"));
  }
};

// Specialization: Get timestamp from the first element if a list
template <typename time_t>
struct timestamp<std::vector<std::string>, time_t> {
  inline static time_t
  from(const std::vector<std::string> &args) {
    return boost::lexical_cast<time_t>(args.at(0));
  }
};



//
//
//
template <typename input_t> struct field_of {
  inline static bool as_bool(const input_t &args, const std::string &key) {
    return args.at(key);
  }
  inline static double as_float(const input_t &args, const std::string &key) {
    return args.at(key);
  }
  inline static std::string as_string(const input_t &args,
                                      const std::string &key) {
    return args.at(key);
  }
  inline static bool as_bool(const input_t &args, std::size_t index) {
    return args.at(index);
  }
  inline static double as_float(const input_t &args, std::size_t index) {
    return args.at(index);
  }
  inline static std::string as_string(const input_t &args, std::size_t index) {
    return args.at(index);
  }
};

template <> struct field_of<std::unordered_map<std::string, std::string>> {
  static const std::unordered_set<std::string> falsity;
  inline static bool
  as_bool(const std::unordered_map<std::string, std::string> &args,
          const std::string &key) {
    return falsity.find(args.at(key)) == falsity.end();
  }
  inline static double
  as_float(const std::unordered_map<std::string, std::string> &args,
           const std::string &key) {
    return boost::lexical_cast<double>(args.at(key));
  }
  inline static std::string
  as_string(const std::unordered_map<std::string, std::string> &args,
            const std::string &key) {
    return args.at(key);
  }
};

template <> struct field_of<std::unordered_map<std::string, long long int>> {
  static const std::unordered_set<std::string> falsity;
  inline static bool
  as_bool(const std::unordered_map<std::string, long long int> &args,
          const std::string &key) {
    return args.at(key);
  }
  inline static double
  as_float(const std::unordered_map<std::string, long long int> &args,
           const std::string &key) {
    return args.at(key);
  }
  inline static std::string
  as_string(const std::unordered_map<std::string, long long int> &args,
            const std::string &key) {
    return std::to_string(args.at(key));
  }
};

template <> struct field_of<std::unordered_map<std::string, long int>> {
  static const std::unordered_set<std::string> falsity;
  inline static bool
  as_bool(const std::unordered_map<std::string, long int> &args,
          const std::string &key) {
    return args.at(key);
  }
  inline static double
  as_float(const std::unordered_map<std::string, long int> &args,
           const std::string &key) {
    return args.at(key);
  }
  inline static std::string
  as_string(const std::unordered_map<std::string, long int> &args,
            const std::string &key) {
    return std::to_string(args.at(key));
  }
};

template <> struct field_of<std::unordered_map<std::string, int>> {
  static const std::unordered_set<std::string> falsity;
  inline static bool as_bool(const std::unordered_map<std::string, int> &args,
                             const std::string &key) {
    return args.at(key);
  }
  inline static double
  as_float(const std::unordered_map<std::string, int> &args,
           const std::string &key) {
    return args.at(key);
  }
  inline static std::string
  as_string(const std::unordered_map<std::string, int> &args,
            const std::string &key) {
    return std::to_string(args.at(key));
  }
};

template <> struct field_of<std::unordered_map<std::string, double>> {
  static const std::unordered_set<std::string> falsity;
  inline static bool
  as_bool(const std::unordered_map<std::string, double> &args,
          const std::string &key) {
    return args.at(key) != 0.0;
  }
  inline static double
  as_float(const std::unordered_map<std::string, double> &args,
           const std::string &key) {
    return args.at(key);
  }
  inline static std::string
  as_string(const std::unordered_map<std::string, double> &args,
            const std::string &key) {
    return std::to_string(args.at(key));
  }
};

template <> struct field_of<std::unordered_map<std::string, float>> {
  static const std::unordered_set<std::string> falsity;
  inline static bool as_bool(const std::unordered_map<std::string, float> &args,
                             const std::string &key) {
    return args.at(key) != 0.0f;
  }
  inline static double
  as_float(const std::unordered_map<std::string, float> &args,
           const std::string &key) {
    return args.at(key);
  }
  inline static std::string
  as_string(const std::unordered_map<std::string, float> &args,
            const std::string &key) {
    return std::to_string(args.at(key));
  }
};

template <> struct field_of<std::unordered_map<std::string, bool>> {
  static const std::unordered_set<std::string> falsity;
  inline static bool as_bool(const std::unordered_map<std::string, bool> &args,
                             const std::string &key) {
    return args.at(key);
  }
  inline static double
  as_float(const std::unordered_map<std::string, bool> &args,
           const std::string &key) {
    return (double) args.at(key);
  }
  inline static std::string
  as_string(const std::unordered_map<std::string, bool> &args,
            const std::string &key) {
    return std::to_string(args.at(key));
  }
};

template <> struct field_of<std::vector<std::string>> {
  static const std::unordered_set<std::string> falsity;
  inline static bool as_bool(const std::vector<std::string> &args,
                             std::size_t index) {
    return falsity.find(args.at(index)) == falsity.end();
  }
  inline static double as_float(const std::vector<std::string> &args,
                                std::size_t index) {
    return boost::lexical_cast<double>(args.at(index));
  }
  inline static std::string as_string(const std::vector<std::string> &args,
                                      std::size_t index) {
    return args.at(index);
  }
};

// Needs a better, more general solution
const std::unordered_set<std::string>
    field_of<std::unordered_map<std::string, std::string>>::falsity = {
        "", "0", "false", "False"};

const std::unordered_set<std::string>
    field_of<std::vector<std::string>>::falsity = {"", "0", "false", "False"};

} // namespace reelay
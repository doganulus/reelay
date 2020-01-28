/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "functional"
#include "memory"
#include "vector"

#include "reelay/common.hpp"
#include "reelay/networks/basic_structure.hpp"
#include "reelay/networks/discrete_timed_network.hpp"

namespace reelay {
namespace discrete_timed_setting {

template <typename X, typename T>
struct proposition : public discrete_timed_state<X, bool, T> {
  using time_t = T;
  using input_t = X;
  using output_t = bool;

  using function_t = std::function<bool(const input_t&)>;

  function_t fn;
  bool value = false;

  explicit proposition(const std::string &name)
      : fn([name](const input_t &x) {
          return boost::lexical_cast<bool>(x.at(name));
        }) {}

  explicit proposition(const kwargs &kw)
      : proposition(reelay::any_cast<std::string>(kw.at("name"))) {}

  void update(const input_t& args, time_t) override { value = fn(args); }

  output_t output(time_t) override { return value; }
};

template <typename X, typename T>
struct basic_predicate_lt : public discrete_timed_state<X, bool, T> {
  using time_t = T;
  using input_t = X;
  using output_t = bool;

  using function_t = std::function<bool(const input_t&)>;

  function_t fn;
  bool value = false;

  basic_predicate_lt(const std::string &name, float c)
      : fn([name, c](const input_t &x) {
          return boost::lexical_cast<float>(x.at(name)) < c;
        }) {}

  explicit basic_predicate_lt(const kwargs &kw)
      : basic_predicate_lt(reelay::any_cast<std::string>(kw.at("name")),
                           reelay::any_cast<float>(kw.at("constant"))) {}

  void update(const input_t& args, time_t) override { value = fn(args); }

  output_t output(time_t) override { return value; }
};

template <typename X, typename T>
struct basic_predicate_le : public discrete_timed_state<X, bool, T> {
  using time_t = T;
  using input_t = X;
  using output_t = bool;

  using function_t = std::function<bool(const input_t&)>;

  function_t fn;
  bool value = false;

  basic_predicate_le(const std::string &name, float c)
      : fn([name, c](const input_t &x) {
          return boost::lexical_cast<float>(x.at(name)) <= c;
        }) {}

  explicit basic_predicate_le(const kwargs &kw)
      : basic_predicate_le(reelay::any_cast<std::string>(kw.at("name")),
                           reelay::any_cast<float>(kw.at("constant"))) {}

  void update(const input_t& args, time_t) override { value = fn(args); }

  output_t output(time_t) override { return value; }
};

template <typename X, typename T>
struct basic_predicate_gt : public discrete_timed_state<X, bool, T> {
  using time_t = T;
  using input_t = X;
  using output_t = bool;

  using function_t = std::function<bool(const input_t&)>;

  function_t fn;
  bool value = false;

  basic_predicate_gt(const std::string &name, float c)
      : fn([name, c](const input_t &x) {
          return boost::lexical_cast<float>(x.at(name)) > c;
        }) {}

  explicit basic_predicate_gt(const kwargs &kw)
      : basic_predicate_gt(reelay::any_cast<std::string>(kw.at("name")),
                           reelay::any_cast<float>(kw.at("constant"))) {}

  void update(const input_t& args, time_t) override { value = fn(args); }

  output_t output(time_t) override { return value; }
};

template <typename X, typename T>
struct basic_predicate_ge : public discrete_timed_state<X, bool, T> {
  using time_t = T;
  using input_t = X;
  using output_t = bool;

  using function_t = std::function<bool(const input_t&)>;

  function_t fn;
  bool value = false;

  basic_predicate_ge(const std::string &name, float c)
      : fn([name, c](const input_t &x) {
          return boost::lexical_cast<float>(x.at(name)) >= c;
        }) {}

  explicit basic_predicate_ge(const kwargs &kw)
      : basic_predicate_ge(reelay::any_cast<std::string>(kw.at("name")),
                           reelay::any_cast<float>(kw.at("constant"))) {}

  void update(const input_t& args, time_t) override { value = fn(args); }

  output_t output(time_t) override { return value; }
};

template <typename X, typename T>
struct predicate : public discrete_timed_state<X, bool, T> {
  using time_t = T;
  using input_t = X;
  using output_t = bool;

  using function_t = std::function<bool(const input_t&)>;

  function_t fn;
  bool value = false;

  explicit predicate(const function_t &f) : fn(f) {}

  explicit predicate(const kwargs &kw)
      : predicate(reelay::any_cast<function_t>(kw.at("function"))) {}

  void update(const input_t& args, time_t) override { value = fn(args); }

  output_t output(time_t) override { return value; }
};

}  // namespace discrete_timed_setting
}  // namespace reelay
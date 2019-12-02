/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "functional"
#include "memory"
#include "vector"

#include "reelay/networks/basic_structure.hpp"
#include "reelay/networks/discrete_timed_network.hpp"

namespace reelay {
namespace discrete_timed_robustness_setting {

template <typename X, typename V, typename T>
struct proposition : public discrete_timed_state<X, V, T> {
  using time_t = T;
  using input_t = X;
  using output_t = V;

  using function_t = std::function<output_t(const input_t&)>;

  output_t value = false;
  function_t fn;

  explicit proposition(const std::string &name)
      : fn([name](const input_t &x) {
          return boost::lexical_cast<V>(x.at(name));
        }) {}

  void update(const input_t& args, time_t now) override { value = fn(args); }

  output_t output(time_t now) override { return value; }
};

template <typename X, typename V, typename T>
struct basic_predicate_lt : public discrete_timed_state<X, V, T> {
  using time_t = T;
  using input_t = X;
  using output_t = V;

  using function_t = std::function<output_t(const input_t &)>;

  output_t value = false;
  function_t fn;

  basic_predicate_lt(const std::string &name, float c)
      : fn([name, c](const input_t &x) {
          return c - boost::lexical_cast<V>(x.at(name));
        }) {}

  void update(const input_t& args, time_t now) override { value = fn(args); }

  output_t output(time_t now) override { return value; }
};

template <typename X, typename V, typename T>
struct basic_predicate_le : public discrete_timed_state<X, V, T> {
  using time_t = T;
  using input_t = X;
  using output_t = V;

  using function_t = std::function<output_t(const input_t &)>;

  output_t value = false;
  function_t fn;

  basic_predicate_le(const std::string &name, float c)
      : fn([name, c](const input_t &x) {
          return c - boost::lexical_cast<V>(x.at(name));
        }) {}

  void update(const input_t& args, time_t now) override { value = fn(args); }

  output_t output(time_t now) override { return value; }
};

template <typename X, typename V, typename T>
struct basic_predicate_gt : public discrete_timed_state<X, V, T> {
  using time_t = T;
  using input_t = X;
  using output_t = V;

  using function_t = std::function<output_t(const input_t &)>;

  output_t value = false;
  function_t fn;

  basic_predicate_gt(const std::string &name, float c)
      : fn([name, c](const input_t &x) {
          return boost::lexical_cast<V>(x.at(name)) - c;
        }) {}

  void update(const input_t& args, time_t now) override { value = fn(args); }

  output_t output(time_t now) override { return value; }
};

template <typename X, typename V, typename T>
struct basic_predicate_ge : public discrete_timed_state<X, V, T> {
  using time_t = T;
  using input_t = X;
  using output_t = V;

  using function_t = std::function<output_t(const input_t &)>;

  output_t value = false;
  function_t fn;

  basic_predicate_ge(const std::string &name, float c)
      : fn([name, c](const input_t &x) {
          return boost::lexical_cast<V>(x.at(name)) - c;
        }) {}

  void update(const input_t& args, time_t now) override { value = fn(args); }

  output_t output(time_t now) override { return value; }
};

template <typename X, typename V, typename T>
struct predicate : public discrete_timed_state<X, V, T> {
  using time_t = T;
  using input_t = X;
  using output_t = V;

  using function_t = std::function<output_t(const input_t &)>;

  output_t value = false;
  function_t fn;

  explicit predicate(const function_t &f) : fn(f) {}

  void update(const input_t& args, time_t now) override { value = fn(args); }

  output_t output(time_t now) override { return value; }
};

} // namespace discrete_timed_robustness_setting
}  // namespace reelay
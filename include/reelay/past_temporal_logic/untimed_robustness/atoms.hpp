/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "string"

#include "reelay/networks/basic_structure.hpp"

namespace reelay {
namespace untimed_robustness_setting {

template <typename X, typename V>
struct proposition : public untimed_state<X, V> {
  using input_t = X;
  using output_t = V;

  using function_t = std::function<output_t(const input_t &)>;

  output_t value = -std::numeric_limits<output_t>::max();
  std::function<output_t(const input_t &)> fn;

  proposition(std::string name)
      : fn([name](const input_t &x) { return std::stof(x.at(name)); }) {}

  void update(const input_t &args) override { value = fn(args); }

  output_t output() override { return value; }
};

template <typename X, typename V>
struct basic_predicate_lt : public untimed_state<X, V> {
  using input_t = X;
  using output_t = V;

  using function_t = std::function<output_t(const input_t &)>;

  output_t value = -std::numeric_limits<output_t>::max();
  std::function<output_t(const input_t &)> fn;

  basic_predicate_lt(std::string name, float c)
      : fn([name, c](const input_t &x) { return c - std::stof(x.at(name)); }) {}

  void update(const input_t &args) override { value = fn(args); }

  output_t output() override { return value; }
};

template <typename X, typename V>
struct basic_predicate_le : public untimed_state<X, V> {
  using input_t = X;
  using output_t = V;

  using Node = untimed_node<output_t>;
  using SharedNode = std::shared_ptr<Node>;

  output_t value = -std::numeric_limits<output_t>::max();
  std::function<output_t(const input_t &)> fn;

  basic_predicate_le(std::string name, float c)
      : fn([name, c](const input_t &x) { return c - std::stof(x.at(name)); }) {}

  void update(const input_t &args) override { value = fn(args); }

  output_t output() override { return value; }
};

template <typename X, typename V>
struct basic_predicate_gt : public untimed_state<X, V> {
  using input_t = X;
  using output_t = V;

  using function_t = std::function<output_t(const input_t &)>;

  output_t value = -std::numeric_limits<output_t>::max();
  std::function<output_t(const input_t &)> fn;

  basic_predicate_gt(std::string name, float c)
      : fn([name, c](const input_t &x) { return std::stof(x.at(name)) - c; }) {}

  void update(const input_t &args) override { value = fn(args); }

  output_t output() override { return value; }
};

template <typename X, typename V>
struct basic_predicate_ge : public untimed_state<X, V> {
  using input_t = X;
  using output_t = V;

  using function_t = std::function<output_t(const input_t &)>;

  output_t value = -std::numeric_limits<output_t>::max();
  std::function<output_t(const input_t &)> fn;

  basic_predicate_ge(std::string name, float c)
      : fn([name, c](const input_t &x) { return std::stof(x.at(name)) - c; }) {}

  void update(const input_t &args) override { value = fn(args); }

  output_t output() override { return value; }
};

template <typename X, typename V>
struct predicate : public untimed_state<X, V> {
  using input_t = X;
  using output_t = V;

  using function_t = std::function<output_t(const input_t &)>;

  output_t value = -std::numeric_limits<output_t>::max();
  std::function<output_t(const input_t &)> fn;

  predicate(const function_t &f) : fn(f) {}

  void update(const input_t &args) override { value = fn(args); }

  output_t output() override { return value; }
};

} // namespace untimed_setting
} // namespace reelay
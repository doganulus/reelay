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
namespace untimed_setting {

template <typename X> struct proposition : public untimed_state<X, bool> {
  using input_t = X;
  using output_t = bool;

  using function_t = std::function<bool(const input_t &)>;

  bool value = false;
  std::function<bool(const input_t &)> fn;

  proposition(std::string name)
      : fn([name](const input_t &x) { return x.at(name) != "0"; }) {}

  void update(const input_t &args) override { value = fn(args); }

  output_t output() override { return value; }
};

template <typename X>
struct basic_predicate_lt : public untimed_state<X, bool> {
  using input_t = X;
  using output_t = bool;

  using function_t = std::function<bool(const input_t &)>;

  bool value = false;
  std::function<bool(const input_t &)> fn;

  basic_predicate_lt(std::string name, float c)
      : fn([name, c](const input_t &x) { return std::stof(x.at(name)) < c; }) {}

  void update(const input_t &args) override { value = fn(args); }

  output_t output() override { return value; }
};

template <typename X>
struct basic_predicate_le : public untimed_state<X, bool> {
  using input_t = X;
  using output_t = bool;

  using Node = untimed_node<output_t>;
  using SharedNode = std::shared_ptr<Node>;

  bool value = false;
  std::function<bool(const input_t &)> fn;

  basic_predicate_le(std::string name, float c)
      : fn([name, c](const input_t &x) { return std::stof(x.at(name)) <= c; }) {
  }

  void update(const input_t &args) override { value = fn(args); }

  output_t output() override { return value; }
};

template <typename X>
struct basic_predicate_gt : public untimed_state<X, bool> {
  using input_t = X;
  using output_t = bool;

  using function_t = std::function<bool(const input_t &)>;

  bool value = false;
  std::function<bool(const input_t &)> fn;

  basic_predicate_gt(std::string name, float c)
      : fn([name, c](const input_t &x) { return std::stof(x.at(name)) > c; }) {}

  void update(const input_t &args) override { value = fn(args); }

  output_t output() override { return value; }
};

template <typename X>
struct basic_predicate_ge : public untimed_state<X, bool> {
  using input_t = X;
  using output_t = bool;

  using function_t = std::function<bool(const input_t &)>;

  bool value = false;
  std::function<bool(const input_t &)> fn;

  basic_predicate_ge(std::string name, float c)
      : fn([name, c](const input_t &x) { return std::stof(x.at(name)) >= c; }) {
  }

  void update(const input_t &args) override { value = fn(args); }

  output_t output() override { return value; }
};

template <typename X> struct predicate : public untimed_state<X, bool> {
  using input_t = X;
  using output_t = bool;

  using function_t = std::function<bool(const input_t &)>;

  bool value = false;
  std::function<bool(const input_t &)> fn;

  predicate(const function_t &f) : fn(f) {}

  void update(const input_t &args) override { value = fn(args); }

  output_t output() override { return value; }
};

} // namespace untimed_setting
} // namespace reelay
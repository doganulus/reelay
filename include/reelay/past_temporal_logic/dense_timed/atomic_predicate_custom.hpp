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


#include "reelay/intervals.hpp"
#include "reelay/networks/basic_structure.hpp"


namespace reelay {
namespace dense_timed_setting {

template <typename X, typename T>
struct predicate : public dense_timed_state<X, interval_set<T>, T> {
  using input_t = X;
  using time_t = T;

  using base_t = reelay::interval_set<time_t>;
  using output_t = reelay::interval_set<time_t>;

  using function_t =
      std::function<output_t(const input_t&, const input_t&, time_t, time_t)>;

  using interval = reelay::interval<time_t>;
  using interval_set = reelay::interval_set<time_t>;

  interval_set value = interval_set();
  function_t fn;

  explicit predicate(const function_t& f) : fn(f) {}

  explicit predicate(const kwargs &kw)
      : predicate(reelay::any_cast<function_t>(kw.at("function"))) {}

  void update(const input_t& pargs,
              const input_t& args,
              time_t previous,
              time_t now) override {
    value = fn(pargs, args, previous, now);
  }

  output_t output(time_t, time_t) override { return value; }
};

}  // namespace dense_timed_setting
}  // namespace reelay
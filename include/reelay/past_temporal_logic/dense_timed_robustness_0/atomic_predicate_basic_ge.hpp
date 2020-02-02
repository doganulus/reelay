#pragma once

#include "functional"
#include "memory"
#include "vector"

#include "reelay/intervals.hpp"
#include "reelay/networks/basic_structure.hpp"

namespace reelay {
namespace dense_timed_robustness_0_setting {

template <typename X, typename V, typename T>
struct basic_predicate_ge
    : public dense_timed_state<X, robustness_interval_map<T, V>, T> {
  using time_t = T;
  using input_t = X;
  using value_t = V;
  using output_t = reelay::robustness_interval_map<time_t, value_t>;

  using node_t = dense_timed_node<output_t, time_t>;
  using state_t = dense_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  using function_t =
      std::function<output_t(const input_t &, const input_t &, time_t, time_t)>;

  using interval = reelay::interval<time_t>;
  using interval_map = reelay::robustness_interval_map<time_t, value_t>;

  interval_map value;
  function_t fn;

  basic_predicate_ge(const std::string &name, float c)
      : fn([name, c](const input_t &, const input_t &args, time_t previous,
                     time_t now) {
          return interval_map(
              std::make_pair(interval::left_open(previous, now),
                             boost::lexical_cast<value_t>(args.at(name)) - c));
        }) {}

  explicit basic_predicate_ge(const kwargs &kw)
      : basic_predicate_ge(reelay::any_cast<std::string>(kw.at("name")),
                           reelay::any_cast<float>(kw.at("constant"))) {}

  void update(const input_t &pargs, const input_t &args, time_t previous,
              time_t now) override {
    value = fn(pargs, args, previous, now);
  }

  output_t output(time_t, time_t) override { return value; }
};

} // namespace dense_timed_robustness_0_setting
} // namespace reelay
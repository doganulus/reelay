/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "reelay/targets/stdout/untimed/stdout_formatter.hpp"
#include "reelay/targets/stdout/untimed/verbosity_0.hpp"
// #include "reelay/targets/stdout/untimed/verbosity_1.hpp"
// #include "reelay/targets/stdout/untimed/verbosity_2.hpp"

#include "reelay/targets/stdout/discrete_timed/stdout_formatter.hpp"
#include "reelay/targets/stdout/discrete_timed/verbosity_0.hpp"
#include "reelay/targets/stdout/discrete_timed/verbosity_1.hpp"
// #include "reelay/targets/stdout/discrete_timed/verbosity_2.hpp"

#include "reelay/targets/stdout/dense_timed/stdout_formatter.hpp"
#include "reelay/targets/stdout/dense_timed/verbosity_0.hpp"
#include "reelay/targets/stdout/dense_timed/verbosity_1.hpp"
// #include "reelay/targets/stdout/dense_timed/verbosity_2.hpp"

namespace reelay {

namespace untimed_setting {

template <typename X>
static std::shared_ptr<stdout_formatter<X>> make_stdout_formatter(
    std::shared_ptr<untimed_network<X, bool>> network,
    std::vector<std::string> header, int verbosity) {

  using input_t = X;

  using fmt_t = stdout_formatter<X>;
  using fmt_ptr_t = std::shared_ptr<fmt_t>;

  fmt_ptr_t formatter;

  // if (verbosity >= 2) {
  //   formatter = std::make_shared<stdout_formatter_verbosity_2<input_t,
  //   time_t>>(
  //       network, header);
  // } else if (verbosity == 1) {
  //   formatter = std::make_shared<stdout_formatter_verbosity_1<input_t,
  //   time_t>>(
  //       network, header);
  // } else {
  formatter = std::make_shared<stdout_formatter_verbosity_0<input_t>>(
      network, header);
  // }
  return formatter;
}

} // namespace discrete_timed_setting
} // namespace reelay

namespace reelay {
namespace discrete_timed_setting {

template <typename X, typename T>
static std::shared_ptr<stdout_formatter<X, T>> make_stdout_formatter(
    std::shared_ptr<discrete_timed_network<X, bool, T>> network,
    std::vector<std::string> header, int verbosity) {

  using time_t = T;
  using input_t = X;

  using fmt_t = stdout_formatter<X, T>;
  using fmt_ptr_t = std::shared_ptr<fmt_t>;

  fmt_ptr_t formatter;

  if (verbosity >= 2) {
    formatter = std::make_shared<stdout_formatter_verbosity_1<input_t, time_t>>(
        network, header);
  } else if (verbosity == 1) {
    formatter = std::make_shared<stdout_formatter_verbosity_1<input_t, time_t>>(
        network, header);
  } else {
    formatter = std::make_shared<stdout_formatter_verbosity_0<input_t, time_t>>(
        network, header);
  }
  return formatter;
}

} // namespace discrete_timed_setting
} // namespace reelay

namespace reelay {
namespace dense_timed_setting {

template <typename X, typename T>
static std::shared_ptr<stdout_formatter<X, T>>
make_stdout_formatter(
    std::shared_ptr<dense_timed_network<X, interval_set<T>, T, bool>> network,
    std::vector<std::string> header, int verbosity) {

  using time_t = T;
  using input_t = X;

  using fmt_t = stdout_formatter<X, T>;
  using fmt_ptr_t = std::shared_ptr<fmt_t>;

  fmt_ptr_t formatter;

  if (verbosity >= 2) {
    formatter = std::make_shared<stdout_formatter_verbosity_1<input_t, time_t>>(
        network, header);
  } else if (verbosity == 1) {
    formatter = std::make_shared<stdout_formatter_verbosity_1<input_t, time_t>>(
        network, header);
  } else {
    formatter = std::make_shared<stdout_formatter_verbosity_0<input_t, time_t>>(
        network, header);
  }
  return formatter;
}
} // namespace dense_timed_setting
} // namespace reelay
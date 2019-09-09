/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

namespace reelay {
namespace discrete_timed_setting {

template <typename X, typename T>
struct stdout_formatter {
  using factory = discrete_timed_setting::factory<X, T>;

  using network_t = typename factory::network_t;

  virtual std::string get_header(const std::vector<std::string>& columm_names);
  virtual std::string get_output(const network_t& network);
};

}  // namespace discrete_timed_setting
}  // namespace reelay
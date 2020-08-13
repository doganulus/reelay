/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>
#include <type_traits>
//
#include "reelay/monitors/abstract_monitor.hpp"

namespace reelay {

template <typename InputT, typename OutputT>
struct monitor {
  using input_type = InputT;
  using output_type = OutputT;

  using type = monitor<input_type, output_type>;
  using impl_type = abstract_monitor<input_type, output_type>;

  output_type now() {
    return pimpl->now();
  }

  output_type update(const input_type& obj) {
    return pimpl->update(obj);
  }

  monitor() {}
  monitor(std::shared_ptr<impl_type> pointer) : pimpl(pointer) {}

 private:
  std::shared_ptr<impl_type> pimpl;
};


}  // namespace reelay
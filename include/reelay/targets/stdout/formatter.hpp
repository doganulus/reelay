/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "reelay/targets/stdout/dense_timed/verbosity_0.hpp"
#include "reelay/targets/stdout/dense_timed/verbosity_1.hpp"
#include "reelay/targets/stdout/dense_timed/verbosity_2.hpp"

namespace reelay {

// template <typename X, typename T>
// struct make_untimed_stdout_formatter {
//   using factory = untimed_setting::factory<X, T>;

//   using input_t = typename factory::input_t;
//   using output_t = typename factory::output_t;
//   using function_t = typename factory::function_t;

//   using network_t = typename factory::network_t;
//   using network_ptr_t = typename factory::network_ptr_t;

//   using fmt_ptr_t =
//       std::shared_ptr<untimed_setting::stdout_formatter<input_t, time_t>>;

//   static fmt_ptr_t with_verbosity(int verbosity) { return stdout_formatter; }
// }

// template <typename X, typename T>
// struct make_discrete_timed_stdout_formatter {
//   using factory = discrete_timed_setting::factory<X, T>;

//   using input_t = typename factory::input_t;
//   using output_t = typename factory::output_t;
//   using function_t = typename factory::function_t;

//   using network_t = typename factory::network_t;
//   using network_ptr_t = typename factory::network_ptr_t;

//   using fmt_ptr_t = std::shared_ptr<
//       discrete_timed_setting::stdout_formatter<input_t, time_t>>;

//   static fmt_ptr_t with_verbosity(int verbosity) { return stdout_formatter; }
// }





// namespace dense_timed_setting {

// template <typename X, typename T>
// struct make_stdout_formatter {
//   using setting = dense_timed_setting::factory<X, T>;

//   using input_t = typename setting::input_t;
//   using output_t = typename setting::output_t;
//   using function_t = typename setting::function_t;

//   using state_t = typename setting::state_t;
//   using state_ptr_t = typename setting::state_ptr_t;

//   using network_t = typename setting::network_t;
//   using network_ptr_t = typename setting::network_ptr_t;

//   static state_ptr_t with_verbosity(int verbosity) {
//     state_ptr_t formatter;

//     if (verbosity >= 2) {
//       formatter = std::make_shared<stdout_formatter_verbosity_2<input_t, time_t>>();
//     } else if (verbosity == 1) {
//       // formatter = std::make_shared<stdout_formatter_verbosity_1<input_t, time_t>>();
//     } else {
//       // formatter = std::make_shared<stdout_formatter_verbosity_0<input_t, time_t>>();
//     }
//     return formatter;
//   }
// };
// }


}  // namespace reelay
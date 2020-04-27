/*
 *                 |
 *    __| _ \  _ \ |  _` | |   |
 *   |    __/  __/ | (   | |   |
 *  _|  \___|\___|_|\__,_|\__, |
 *                         ____/    Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "functional"
#include "map"
#include "memory"
#include "string"

#include "reelay/common.hpp"
#include "reelay/parser/ptl.hpp"
// #include "reelay/parser/regex.hpp"
#include "reelay/settings.hpp"

#include "reelay/intervals.hpp"

namespace reelay {
namespace detail {

template <typename input_t> struct network {
  using factory = untimed_setting::factory<input_t>;

  using network_t = typename factory::network_t;
  using network_ptr_t = typename factory::network_ptr_t;

  static network_ptr_t
  from_temporal_logic(std::string pattern,
                      reelay::kwargs kw = reelay::kwargs()) {
    auto parser = ptl_parser<factory>(kw);
    return parser.parse(pattern);
  }
  // static network_ptr_t
  // from_regular_expressions(std::string pattern,
  //                          reelay::kwargs kw = reelay::kwargs()) {
  //   auto parser = regex_parser<factory>(kw);
  //   return parser.parse(pattern);
  // }
};

struct unordered_data {
  template <typename input_t> struct network {
    using factory = untimed_data_setting::factory<input_t>;

    using network_t = typename factory::network_t;
    using network_ptr_t = typename factory::network_ptr_t;

    static network_ptr_t
    from_temporal_logic(std::string pattern,
                        reelay::kwargs kw = reelay::kwargs()) {
      auto parser = ptl_parser<factory>(kw);
      return parser.parse(pattern);
    }
  };
};

template <typename output_t> struct robustness {
  template <typename input_t> struct network {
    using factory = untimed_robustness_setting::factory<input_t, output_t>;

    using network_t = typename factory::network_t;
    using network_ptr_t = typename factory::network_ptr_t;

    static network_ptr_t
    from_temporal_logic(std::string pattern,
                        reelay::kwargs kw = reelay::kwargs()) {
      auto parser = ptl_parser<factory>(kw);
      return parser.parse(pattern);
    }
  };
};

template <typename time_t> struct discrete_timed {
  template <typename input_t> struct network {
    using factory = discrete_timed_setting::factory<input_t, time_t>;

    using network_t = typename factory::network_t;
    using network_ptr_t = typename factory::network_ptr_t;

    static network_ptr_t
    from_temporal_logic(std::string pattern,
                        reelay::kwargs kw = reelay::kwargs()) {
      auto parser = ptl_parser<factory>(kw);
      return parser.parse(pattern);
    }
    // static network_ptr_t from_regular_expressions();
  };

  struct unordered_data {
    template <typename input_t> struct network {
      using factory = discrete_timed_data_setting::factory<input_t, time_t>;

      using network_t = typename factory::network_t;
      using network_ptr_t = typename factory::network_ptr_t;

      static network_ptr_t
      from_temporal_logic(std::string pattern,
                          reelay::kwargs kw = reelay::kwargs()) {
        auto parser = ptl_parser<factory>(kw);
        return parser.parse(pattern);
      }
    };
  };

  template <typename output_t> struct robustness {
    template <typename input_t> struct network {
      using factory =
          discrete_timed_robustness_setting::factory<input_t, output_t, time_t>;

      using network_t = typename factory::network_t;
      using network_ptr_t = typename factory::network_ptr_t;

      static network_ptr_t
      from_temporal_logic(std::string pattern,
                          reelay::kwargs kw = reelay::kwargs()) {
        auto parser = ptl_parser<factory>(kw);
        return parser.parse(pattern);
      }
    };
  };
};

template <typename time_t, int option = 0> struct dense_timed {
  template <typename input_t> struct network {
    using factory = dense_timed_setting::factory<input_t, time_t, option>;

    using network_t = typename factory::network_t;
    using network_ptr_t = typename factory::network_ptr_t;

    static network_ptr_t
    from_temporal_logic(std::string pattern,
                        reelay::kwargs kw = reelay::kwargs()) {
      auto parser = ptl_parser<factory>(kw);
      return parser.parse(pattern);
    }

    // static type from_regular_expressions();
  };

  struct unordered_data {
    template <typename input_t> struct network {
      using factory =
          dense_timed_data_setting::factory<input_t, time_t, option>;

      using network_t = typename factory::network_t;
      using network_ptr_t = typename factory::network_ptr_t;

      static network_ptr_t
      from_temporal_logic(std::string pattern,
                          reelay::kwargs kw = reelay::kwargs()) {
        auto parser = ptl_parser<factory>(kw);
        return parser.parse(pattern);
      }
    };
  };

  template <typename value_t> struct robustness {
    template <typename input_t> struct network {
      using factory =
          dense_timed_robustness_0_setting::factory<input_t, value_t, time_t>;

      using network_t = typename factory::network_t;
      using network_ptr_t = typename factory::network_ptr_t;

      static network_ptr_t
      from_temporal_logic(std::string pattern,
                          reelay::kwargs kw = reelay::kwargs()) {
        auto parser = ptl_parser<factory>(kw);
        return parser.parse(pattern);
      }
    };
  };
};
}  // namespace detail
}  // namespace reelay

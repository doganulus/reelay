/*
 * Copyright (c) 2019-2023 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "boost/icl/interval.hpp"
#include "boost/icl/interval_map.hpp"
#include "boost/icl/interval_set.hpp"

#include "cuddObj.hh"

namespace boost {
namespace icl {

/*
 * Inplace BDD OR functor
 */
template<typename V>
struct data_join {
  using first_argument_type = V&;
  using second_argument_type = const V&;

  using result_type = void;

  using type = data_join<V>;

  inline static V identity_element()
  {
    // This static function is unsafe and causes a segfault if accessed.
    // By design Boost ICL requires this function to exist and be static.
    //
    // The main problem: The identity element for BDDs, `false` node for join
    // operation, cannot be defined at compile time as we need a BDD manager
    // first, only constructed at runtime. Alternative solution would be to
    // define a singleton manager but it would hurt flexibility deeply.
    //
    // For long term we need to write our own interval_map implementation.
    throw std::runtime_error("Identity element shall not have been accessed!");
    return BDD();
  }  // Unsafe

  void operator()(V& object, const V& operand) const
  {
    object |= operand;
  }
};

/*
 * Inplace BDD AND functor
 */
template<typename V>
struct data_meet {
  using first_argument_type = V&;
  using second_argument_type = const V&;

  using result_type = void;

  using type = data_meet<V>;

  inline static V identity_element()
  {
    // This static function is unsafe and causes a segfault if accessed.
    // By design Boost ICL requires this function to exist and be static.
    //
    // The main problem: The identity element for BDDs, `true` node for meet
    // operation, cannot be defined at compile time as we need a BDD manager
    // first, only constructed at runtime. Alternative solution would be to
    // define a singleton manager but it would hurt flexibility deeply.
    //
    // For long term we need to write our own interval_map implementation.
    throw std::runtime_error("Identity element shall not have been accessed!");
    return BDD();
  }

  void operator()(V& object, const V& operand) const
  {
    object &= operand;
  }
};

/*
 * Boost ICL uses operator+ for Combiner functor (here data_join)
 */
template<class T>
struct inverse<boost::icl::data_join<T>> {
  using type = boost::icl::data_meet<T>;
};

/*
 * Boost ICL uses operator- for the inverse of Combiner functor (here data_meet)
 */
template<class T>
struct inverse<boost::icl::data_meet<T>> {
  using type = boost::icl::data_join<T>;
};

/*
 * Inplace MAX functor
 */
template<typename V>
struct robustness_join {
  using first_argument_type = V&;
  using second_argument_type = const V&;

  using result_type = void;

  using type = robustness_join<V>;

  inline static V identity_element()
  {
    return -infinity<V>::value();
  }

  void operator()(V& object, const V& operand) const
  {
    if(object < operand) {
      object = operand;
    }
  }
};

/*
 * Inplace MIN functor
 */
template<typename V>
struct robustness_meet {
  using first_argument_type = V&;
  using second_argument_type = const V&;

  using result_type = void;

  using type = robustness_meet<V>;

  inline static V identity_element()
  {
    return infinity<V>::value();
  }

  void operator()(V& object, const V& operand) const
  {
    if(object > operand) {
      object = operand;
    }
  }
};

/*
 * Boost ICL uses operator+ for Combiner functor
 * (here robustness_join)
 */
template<class T>
struct inverse<robustness_meet<T>> {
  using type = robustness_join<T>;
};

/*
 * Boost ICL uses operator- for the inverse of Combiner functor
 * (here robustness_meet)
 */
template<class T>
struct inverse<robustness_join<T>> {
  using type = robustness_meet<T>;
};

}  // namespace icl
}  // namespace boost

namespace reelay {

template<class V>
using infinity =
  boost::icl::infinity<V>;  // aka arbitrarily large finite number

template<class T>
using interval = boost::icl::interval<T>;

/*
 * This is the base data structure for timed Boolean monitors
 * It represents a Boolean signal, a map from the time domain to Boolean
 * domain, but intervals mapped to `false` are not included explicitly.
 */
template<class T>
using interval_set = boost::icl::interval_set<T>;

/*
 * This is the base data structure for timed robustness monitors
 * It represents a robustness signal, a map from the time domain to a numerical
 * value domain called robust satisfaction degree.
 */
template<class T, class V>
using robustness_interval_map = boost::icl::interval_map<
  T,
  V,
  boost::icl::total_enricher,
  std::less,
  boost::icl::robustness_join>;

/*
 * This is the base data structure for timed unordered data monitors
 * It represents a BDD signal, a map from the time domain to BDDs, which
 * encode sets of assignments between a number of data variables and elements.
 */
template<class T>
using data_interval_map = boost::icl::interval_map<
  T,
  BDD,
  boost::icl::total_enricher,
  std::less,
  boost::icl::data_join>;
}  // namespace reelay

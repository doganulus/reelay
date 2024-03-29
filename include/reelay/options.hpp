/*
 * Copyright (c) 2019-2023 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>
#include <utility>
//
#include "reelay/json.hpp"
#include "reelay/unordered_data.hpp"

namespace reelay {

// enum class time_model : int { error = 0, discrete = 1, dense = 2 };
// enum class value_model : int { error = 0, boolean = 1, robustness = 2 };
enum class piecewise : int { error = 0, constant = 1, linear = 2 };

struct basic_options {
  basic_options() = default;

  basic_options(
    std::string tname, std::string yname, bool condense, bool with_data_manager)
      : time_field_name(std::move(tname)),
        value_field_name(std::move(yname)),
        condensing(condense)
  {
    if(with_data_manager) {
      data_manager = std::make_shared<reelay::binding_manager>();
    }
  }

  basic_options(
    std::string tname,
    std::string yname,
    const std::string& interpolation,
    bool with_data_manager)
      : time_field_name(std::move(tname)), value_field_name(std::move(yname))
  {
    if(interpolation == "constant") {
      interpolation_option = piecewise::constant;
    }
    else if(interpolation == "linear") {
      interpolation_option = piecewise::linear;
    }
    if(with_data_manager) {
      data_manager = std::make_shared<reelay::binding_manager>();
    }
  }

  basic_options& with_interpolation(piecewise order)
  {
    interpolation_option = order;
    return *this;
  }

  basic_options& with_data_manager()
  {
    data_manager = std::make_shared<reelay::binding_manager>();
    return *this;
  }

  basic_options& with_data_manager(const data_mgr_t& mgr)
  {
    data_manager = mgr;
    return *this;
  }

  basic_options& with_condensing(bool flag)
  {
    condensing = flag;
    return *this;
  }

  basic_options& enable_condensing()
  {
    condensing = true;
    return *this;
  }

  basic_options& disable_condensing()
  {
    condensing = false;
    return *this;
  }

  basic_options& with_time_field_name(const std::string& name)
  {
    time_field_name = name;
    return *this;
  }

  basic_options& with_value_field_name(const std::string& name)
  {
    value_field_name = name;
    return *this;
  }

  [[nodiscard]] std::string get_time_field_name() const
  {
    return time_field_name;
  }

  [[nodiscard]] std::string get_value_field_name() const
  {
    return value_field_name;
  }

  [[nodiscard]] data_mgr_t get_data_manager() const
  {
    return data_manager;
  }

  [[nodiscard]] piecewise get_interpolation() const
  {
    return interpolation_option;
  }

  [[nodiscard]] bool is_condensing() const
  {
    return condensing;
  }

 private:
  data_mgr_t data_manager;

  // Setting options

  enum piecewise interpolation_option = piecewise::constant;

  // Formatter options
  bool condensing = true;
  std::string time_field_name = "time";
  std::string value_field_name = "value";
};

template<typename T, typename X, typename Y>
struct dense_monitor_options {
  using time_type = T;
  using input_type = X;
  using output_type = Y;

  dense_monitor_options() = default;

  dense_monitor_options& with_interpolation(piecewise order)
  {
    options.with_interpolation(order);
    return *this;
  }

  dense_monitor_options& with_data_manager()
  {
    options.with_data_manager();
    return *this;
  }

  dense_monitor_options& with_data_manager(const data_mgr_t& mgr)
  {
    options.with_data_manager(mgr);
    return *this;
  }

  dense_monitor_options& with_time_field_name(const std::string& name)
  {
    options.with_time_field_name(name);
    return *this;
  }

  dense_monitor_options& with_value_field_name(const std::string& name)
  {
    options.with_value_field_name(name);
    return *this;
  }

  [[nodiscard]] piecewise get_interpolation() const
  {
    return options.get_interpolation();
  }

  [[nodiscard]] std::string get_time_field_name() const
  {
    return options.get_time_field_name();
  }

  [[nodiscard]] std::string get_value_field_name() const
  {
    return options.get_value_field_name();
  }

  [[nodiscard]] basic_options get_basic_options() const
  {
    return options;
  }

 private:
  basic_options options;
};

template<typename T, typename V, typename X, typename Y>
struct dense_robustness_monitor_options {
  using time_type = T;
  using value_type = V;
  using input_type = X;
  using output_type = Y;

  dense_robustness_monitor_options() = default;

  dense_robustness_monitor_options& with_time_field_name(
    const std::string& name)
  {
    options.with_time_field_name(name);
    return *this;
  }

  dense_robustness_monitor_options& with_value_field_name(
    const std::string& name)
  {
    options.with_value_field_name(name);
    return *this;
  }

  [[nodiscard]] std::string get_time_field_name() const
  {
    return options.get_time_field_name();
  }

  [[nodiscard]] std::string get_value_field_name() const
  {
    return options.get_value_field_name();
  }

  [[nodiscard]] basic_options get_basic_options() const
  {
    return options;
  }

 private:
  basic_options options;
};

template<typename T, typename X, typename Y>
struct discrete_monitor_options {
  using time_type = T;
  using input_type = X;
  using output_type = Y;

  discrete_monitor_options() = default;

  discrete_monitor_options& with_data_manager()
  {
    options.with_data_manager();
    return *this;
  }

  discrete_monitor_options& with_data_manager(const data_mgr_t& mgr)
  {
    options.with_data_manager(mgr);
    return *this;
  }

  discrete_monitor_options& with_time_field_name(const std::string& name)
  {
    options.with_time_field_name(name);
    return *this;
  }

  discrete_monitor_options& with_value_field_name(const std::string& name)
  {
    options.with_value_field_name(name);
    return *this;
  }

  discrete_monitor_options& with_condensing(bool flag)
  {
    options.with_condensing(flag);
    return *this;
  }

  discrete_monitor_options& enable_condensing()
  {
    options.enable_condensing();
    return *this;
  }

  discrete_monitor_options& disable_condensing()
  {
    options.disable_condensing();
    return *this;
  }

  [[nodiscard]] bool is_condensing() const
  {
    return options.is_condensing();
  }

  [[nodiscard]] std::string get_time_field_name() const
  {
    return options.get_time_field_name();
  }

  [[nodiscard]] std::string get_value_field_name() const
  {
    return options.get_value_field_name();
  }

  [[nodiscard]] basic_options get_basic_options() const
  {
    return options;
  }

 private:
  basic_options options;
};

template<typename T, typename V, typename X, typename Y>
struct discrete_robustness_monitor_options {
  using time_type = T;
  using value_type = V;
  using input_type = X;
  using output_type = Y;

  discrete_robustness_monitor_options() = default;

  discrete_robustness_monitor_options& with_time_field_name(
    const std::string& name)
  {
    options.with_time_field_name(name);
    return *this;
  }

  discrete_robustness_monitor_options& with_value_field_name(
    const std::string& name)
  {
    options.with_value_field_name(name);
    return *this;
  }

  discrete_robustness_monitor_options& with_condensing(bool flag)
  {
    options.with_condensing(flag);
    return *this;
  }

  discrete_robustness_monitor_options& enable_condensing()
  {
    options.enable_condensing();
    return *this;
  }

  discrete_robustness_monitor_options& disable_condensing()
  {
    options.disable_condensing();
    return *this;
  }

  [[nodiscard]] bool is_condensing() const
  {
    return options.is_condensing();
  }

  [[nodiscard]] std::string get_time_field_name() const
  {
    return options.get_time_field_name();
  }

  [[nodiscard]] std::string get_value_field_name() const
  {
    return options.get_value_field_name();
  }

  [[nodiscard]] basic_options get_basic_options() const
  {
    return options;
  }

 private:
  basic_options options;
};

}  // namespace reelay

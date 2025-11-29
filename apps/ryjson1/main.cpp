/*
 * Copyright (c) 2019-2025 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "reelay/json.hpp"
#include "reelay/monitors.hpp"

#include <array>
#include <cstring>  // memset()
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "simdjson.h"
#include "simdjson_adapter.hpp"
#include <argp.h>

namespace rycli {

template<typename X, typename Y>
void discrete_timed_processing(
  reelay::monitor<X, Y>& monitor, const std::string& filename)
{
  int stdout_line_count = 0;
  simdjson::dom::parser reader;
  std::ofstream output_file(filename + ".ryl");
  std::cout << "Processing " + filename << std::endl;
  std::cout << "---" << std::endl;
  for(simdjson::dom::element doc : reader.load_many(filename)) {
    reelay::json result = monitor.update(doc);
    if(not result.empty()) {
      if(stdout_line_count < 5) {
        std::cout << result << std::endl;
        stdout_line_count++;
      }
      else if(stdout_line_count == 5) {
        std::cout << "..." << std::endl;
        stdout_line_count++;
      }
      output_file << result << std::endl;
    }
  }
  if(stdout_line_count < 5) {
    std::cout << "---" << std::endl;
  }
  std::cout << "Full output written to " + filename + ".ryl" << std::endl;
}

template<typename X, typename Y>
void dense_timed_processing(
  reelay::monitor<X, Y>& monitor, const std::string& filename)
{
  int stdout_line_count = 0;
  simdjson::dom::parser reader;
  std::ofstream output_file(filename + ".ryl");
  std::cout << "Processing " + filename << std::endl;
  std::cout << "---" << std::endl;
  for(simdjson::dom::element doc : reader.load_many(filename)) {
    reelay::json result = monitor.update(doc);
    if(not result.empty()) {
      for(const auto& item : result) {
        if(stdout_line_count < 5) {
          std::cout << item << std::endl;
          stdout_line_count++;
        }
        else if(stdout_line_count == 5) {
          std::cout << "..." << std::endl;
          stdout_line_count++;
        }
        output_file << item << std::endl;
      }
    }
  }
  if(stdout_line_count < 5) {
    std::cout << "---" << std::endl;
  }
  std::cout << "Full output written to " + filename + ".ryl" << std::endl;
}

}  // namespace rycli

// argp option keys
enum {
  OPT_DENSE = 'v',
  OPT_DISCRETE = 'x',
  OPT_ITIME = 'i',
  OPT_FTIME = 'f',
  OPT_BOOLEAN = 'b',
  OPT_ROBUSTNESS = 'r',
  OPT_PWC = 'k',
  OPT_PWL = 'l',
  OPT_NO_CONDENSE = 'z',
  OPT_TNAME = 1000,
  OPT_YNAME
};

const char* argp_program_version = "ryjson 1.0";
const char* argp_program_bug_address = "<doganulus@gmail.com>";
static const char* doc =
  "Reelay Command Line Interface\nFurther information: "
  "https://github.com/doganulus/reelay";
static const char* args_doc = "SPEC FILE...";

struct arguments {
  char* spec;
  std::vector<std::string> files;
  bool dense = false;
  bool discrete = false;
  bool itime = false;
  bool ftime = false;
  bool boolean = false;
  bool robustness = false;
  bool pwc = false;
  bool pwl = false;
  bool no_condense = false;
  std::string tname = "time";
  std::string yname = "value";
};

static std::array<struct argp_option, 12> options = {
  {{"dense", OPT_DENSE, nullptr, 0, "Use dense time model (default)", 0},
   {"discrete", OPT_DISCRETE, nullptr, 0, "Use discrete time model", 0},
   {"itime", OPT_ITIME, nullptr, 0, "Use int64 as time type (default)", 0},
   {"ftime", OPT_FTIME, nullptr, 0, "with -v, use float64 as time type", 0},
   {"boolean", OPT_BOOLEAN, nullptr, 0, "Use boolean semantics", 0},
   {"robustness", OPT_ROBUSTNESS, nullptr, 0, "Use robustness semantics", 0},
   {"pwc",
    OPT_PWC,
    nullptr,
    0,
    "with -v, use piecewise constant interpolation (default)",
    0},
   {"pwl",
    OPT_PWL,
    nullptr,
    0,
    "with -vf, use piecewise linear interpolation",
    0},
   {"no-condense",
    OPT_NO_CONDENSE,
    nullptr,
    0,
    "with -x, disable dense output",
    0},
   {"tname", OPT_TNAME, "STRING", 0, "Use STRING as the name of time field", 0},
   {"yname",
    OPT_YNAME,
    "STRING",
    0,
    "Use STRING as the name of output field",
    0},
   {nullptr}}};

static error_t parse_opt(int key, char* arg, struct argp_state* state)
{
  auto* arguments = (struct arguments*)state->input;
  switch(key) {
    case OPT_DENSE:
      arguments->dense = true;
      break;
    case OPT_DISCRETE:
      arguments->discrete = true;
      break;
    case OPT_ITIME:
      arguments->itime = true;
      break;
    case OPT_FTIME:
      arguments->ftime = true;
      break;
    case OPT_BOOLEAN:
      arguments->boolean = true;
      break;
    case OPT_ROBUSTNESS:
      arguments->robustness = true;
      break;
    case OPT_PWC:
      arguments->pwc = true;
      break;
    case OPT_PWL:
      arguments->pwl = true;
      break;
    case OPT_NO_CONDENSE:
      arguments->no_condense = true;
      break;
    case OPT_TNAME:
      arguments->tname = arg;
      break;
    case OPT_YNAME:
      arguments->yname = arg;
      break;
    case ARGP_KEY_ARG:
      if(state->arg_num == 0) {
        arguments->spec = arg;
      }
      else {
        arguments->files.emplace_back(arg);
      }
      break;
    case ARGP_KEY_END:
      if(state->arg_num < 2) {
        argp_usage(state);
      }
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}
static struct argp argp = {options.data(), parse_opt, args_doc, doc};

int main(int argc, char** argv)
{
  using namespace reelay;

  struct arguments arguments;
  argp_parse(&argp, argc, argv, 0, nullptr, &arguments);

  using input_t = simdjson::dom::element;
  using output_t = reelay::json;

  // Choices
  bool use_discrete = arguments.discrete;
  bool use_dense = arguments.dense;
  bool use_boolean = arguments.boolean;
  bool use_robustness = arguments.robustness;
  bool use_integer = arguments.itime;
  bool use_floating = arguments.ftime;
  bool use_constant = arguments.pwc;
  bool use_linear = arguments.pwl;

  // Apply defaults
  if(!use_discrete && !use_dense) {
    use_discrete = true;
  }
  if(!use_boolean && !use_robustness) {
    use_boolean = true;
  }
  if(!use_integer && !use_floating) {
    use_integer = true;
  }
  if(!use_constant && !use_linear) {
    use_constant = true;
  }

  using input_t = simdjson::dom::element;
  using output_t = reelay::json;

  auto manager = std::make_shared<reelay::binding_manager>();
  auto monitor = reelay::monitor<input_t, output_t>();

  if(use_discrete && use_boolean) {  // -x -xb -xbz
    auto opts =
      reelay::discrete_timed<int64_t>::monitor<input_t, output_t>::options()
        .with_time_field_name(arguments.tname)
        .with_value_field_name(arguments.yname)
        .with_condensing(!arguments.no_condense)
        .with_data_manager(manager);
    monitor = reelay::make_monitor(arguments.spec, opts);
  }
  else if(use_discrete && use_robustness) {  // -xr -xrz
    auto opts = reelay::discrete_timed<int64_t>::robustness<
                  double>::monitor<input_t, output_t>::options()
                  .with_time_field_name(arguments.tname)
                  .with_value_field_name(arguments.yname)
                  .with_condensing(!arguments.no_condense);
    monitor = reelay::make_monitor(arguments.spec, opts);
  }
  else if(use_dense && use_boolean && use_integer) {  // -vbi
    auto opts =
      reelay::dense_timed<int64_t>::monitor<input_t, output_t>::options()
        .with_time_field_name(arguments.tname)
        .with_value_field_name(arguments.yname)
        .with_data_manager(manager);
    monitor = reelay::make_monitor(arguments.spec, opts);
  }
  else if(use_dense && use_boolean && use_floating && use_constant) {  // -vbf,
                                                                       // -vbfk
    auto opts =
      reelay::dense_timed<double>::monitor<input_t, output_t>::options()
        .with_time_field_name(arguments.tname)
        .with_value_field_name(arguments.yname)
        .with_interpolation(reelay::piecewise::constant)
        .with_data_manager(manager);
    monitor = reelay::make_monitor(arguments.spec, opts);
  }
  else if(use_dense && use_boolean && use_floating && use_linear) {  // -vbfl
    auto opts =
      reelay::dense_timed<double>::monitor<input_t, output_t>::options()
        .with_time_field_name(arguments.tname)
        .with_value_field_name(arguments.yname)
        .with_interpolation(reelay::piecewise::linear)
        .with_data_manager(manager);
    monitor = reelay::make_monitor(arguments.spec, opts);
  }
  else if(use_dense && use_robustness && use_integer) {  // -vri
    auto opts = reelay::dense_timed<int64_t>::robustness<
                  double>::monitor<input_t, output_t>::options()
                  .with_time_field_name(arguments.tname)
                  .with_value_field_name(arguments.yname);
    monitor = reelay::make_monitor(arguments.spec, opts);
  }
  else if(use_dense && use_robustness && use_floating) {  // -vrf
    auto opts = reelay::dense_timed<double>::robustness<
                  double>::monitor<input_t, output_t>::options()
                  .with_time_field_name(arguments.tname)
                  .with_value_field_name(arguments.yname);
    monitor = reelay::make_monitor(arguments.spec, opts);
  }
  else {
    throw std::invalid_argument("Unsupported flag combination");
  }

  if(use_discrete) {
    for(const auto& filename : arguments.files) {
      rycli::discrete_timed_processing(monitor, filename);
    }
  }
  else {
    for(const auto& filename : arguments.files) {
      rycli::dense_timed_processing(monitor, filename);
    }
  }

  return 0;
}

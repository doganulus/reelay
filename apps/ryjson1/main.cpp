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
#include <glob.h>    // glob(), globfree()
#include <string.h>  // memset()

#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "reelay/json.hpp"
#include "reelay/monitors.hpp"

#include "third_party/simdjson/simdjson.h"
#include "third_party/simdjson/simdjson.cpp"
#include "third_party/taywee/args.hpp"

#include "simdjson_adapter.hpp"

namespace rycli {

std::vector<std::string> expand_glob(const std::string& glob_str) {

  // glob struct resides on the stack
  glob_t glob_result;
  memset(&glob_result, 0, sizeof(glob_result));

  // do the glob operation
  int return_value
      = glob(glob_str.c_str(), GLOB_TILDE, NULL, &glob_result);

  // glob() error handling
  // Info: http://man7.org/linux/man-pages/man3/glob.3.html
  if (return_value == GLOB_ABORTED) {
    globfree(&glob_result);
    std::stringstream ss;
    ss << "glob() encountered a read error" << std::endl;
    throw std::runtime_error(ss.str());
  } else if (return_value == GLOB_NOSPACE) {
    globfree(&glob_result);
    std::stringstream ss;
    ss << "glob() running out of memory" << std::endl;
    throw std::runtime_error(ss.str());
  }

  // collect all the filenames into a std::list<std::string>
  std::vector<std::string> filenames;
  for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
    filenames.push_back(std::string(glob_result.gl_pathv[i]));
  }

  // cleanup
  globfree(&glob_result);

  // done
  return filenames;
}

template <typename X, typename Y>
void discrete_timed_processing(reelay::monitor<X, Y>& monitor, const std::string& filename) {
  int stdout_line_count = 0;
  simdjson::dom::parser reader;
  std::ofstream output_file(filename + ".ryl");
  std::cout << "Processing " + filename << std::endl;
  std::cout << "---" << std::endl;
  for (simdjson::dom::element doc : reader.load_many(filename)) {
    reelay::json result = monitor.update(doc);
    if (not result.empty()) {
      if (stdout_line_count < 5) {
        std::cout << result << std::endl;
        stdout_line_count++;
      } else if (stdout_line_count == 5) {
        std::cout << "..." << std::endl;
        stdout_line_count++;
      }
      output_file << result << std::endl;
    }
  }
  if (stdout_line_count < 5) {
    std::cout << "---" << std::endl;
  }
  std::cout << "Full output written to " + filename + ".ryl" << std::endl;
}

template<typename X, typename Y>
void dense_timed_processing(reelay::monitor<X, Y>& monitor, const std::string& filename) {
  int stdout_line_count = 0;
  simdjson::dom::parser reader;
  std::ofstream output_file(filename + ".ryl");
  std::cout << "Processing " + filename << std::endl;
  std::cout << "---" << std::endl;
  for (simdjson::dom::element doc : reader.load_many(filename)) {
    reelay::json result = monitor.update(doc);
    if (not result.empty()) {
      for (const auto& item : result) {
        if (stdout_line_count < 5) {
          std::cout << item << std::endl;
          stdout_line_count++;
        } else if (stdout_line_count == 5) {
          std::cout << "..." << std::endl;
          stdout_line_count++;
        }
        output_file << item << std::endl;
      }
    }
  }
  if (stdout_line_count < 5) {
    std::cout << "---" << std::endl;
  }
  std::cout << "Full output written to " + filename + ".ryl" << std::endl;
}

}  // namespace rycli

int main(int argc, char** argv) {

  using namespace reelay;

  //defaults

  // argparser
  args::ArgumentParser parser(
      "Reelay Command Line Interface",
      "Further information: https://github.com/doganulus/reelay");

  args::Positional<std::string> spec(
      parser, "SPEC", "Specification in Rye Format", args::Options::Required);
  args::PositionalList<std::string> paths(
      parser, "FILE", "Log files to be checked against SPEC",
      args::Options::Required);

  args::Group gsetting(parser, "Monitor Settings");

  args::Group gmodel(gsetting, "Time model:", args::Group::Validators::Xor,
                     args::Options::Global);
  args::Flag fdense(gmodel, "fv", "Use dense time model (default)",
                {'v', "dense"});
  args::Flag fdiscrete(gmodel, "fx", "Use discrete time model",
                {'x', "discrete"});

  args::Group gdtype(gsetting, "Time datatype:", args::Group::Validators::Xor,
                     args::Options::Global);
  args::Flag fint(gdtype, "fi", "Use int64 as time type (default)",
                {'i', "itime"});
  args::Flag ffloat(gdtype, "ff", "with -v, use float64 as time type",
                {'f', "ftime"});

  args::Group gsemantics(gsetting, "Value model:", args::Group::Validators::Xor,
                         args::Options::Global);
  args::Flag fboolean(
      gsemantics, "fb", "Use boolean semantics", {'b', "boolean"});
  args::Flag frobustness(
      gsemantics, "fr", "Use robustness semantics", {'r', "robustness"});

  args::Group ginterp(gsetting, "Interpolation:", args::Group::Validators::Xor,
                      args::Options::Global);
  args::Flag fconstant(ginterp, "fk",
                "with -v, use piecewise constant interpolation (default)",
                {'k', "pwc"});
  args::Flag flinear(ginterp, "fl", "with -vf, use piecewise linear interpolation",
                {'l', "pwl"});

  args::Flag fno_condense(gsetting, "fno-condense",
                          "with -x, disable dense output", {'z', "no-condense"});

  args::Group gioctrl(parser, "Input/Output Control");
  args::Group gformat(gioctrl,
                      "Input File Format:", args::Group::Validators::Xor,
                      args::Options::Global);

  args::ValueFlag<std::string> t_name(
      gioctrl, "STRING", "Use STRING as the name of time field", {"tname"}, "time");
  args::ValueFlag<std::string> y_name(
      gioctrl, "STRING", "use STRING as the name of output field", {"yname"}, "value");

  args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});

  args::CompletionFlag completion(parser, {"complete"});




  try {
    parser.ParseCLI(argc, argv);
  } catch (const args::Completion& e) {
    std::cout << e.what();
    return 0;
  } catch (const args::Help&) {
    std::cout << parser;
    return 0;
  } catch (const args::ParseError& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << parser;
    return 1;
  }

  // Choices
  bool use_discrete = args::get(fdiscrete);
  bool use_dense = args::get(fdense);

  bool use_boolean = args::get(fboolean);
  bool use_robustness = args::get(frobustness);

  bool use_integer = args::get(fint);
  bool use_floating = args::get(ffloat);

  bool use_constant = args::get(fconstant);
  bool use_linear = args::get(flinear);

  // Apply defaults
  if (not use_discrete and not use_dense) {
    use_discrete = true;
  }

  if (not use_boolean and not use_robustness) {
    use_boolean = true;
  }

  if (not use_integer and not use_floating) {
    use_integer = true;
  }

  if (not use_constant and not use_linear) {
    use_constant = true;
  }

  // Glob Expansion
  auto filenames = std::vector<std::string>();
  for (const auto& p : args::get(paths)) {
    std::vector<std::string> expanded = rycli::expand_glob(p);
    filenames.insert(filenames.end(), expanded.begin(), expanded.end());
  }

  using input_t = simdjson::dom::element;
  using output_t = reelay::json;

  auto monitor = reelay::monitor<input_t, output_t>();

  if (use_discrete and use_boolean) {  // -xb -xbz
    auto opts = reelay::discrete_timed<intmax_t>::monitor<
                    input_t, output_t>::options()
                    .with_time_field_name(args::get(t_name))
                    .with_value_field_name(args::get(y_name))
                    .with_condensing(not args::get(fno_condense));

    monitor = reelay::make_monitor(args::get(spec), opts);

  } else if (use_discrete and use_robustness) {  // -xr -xrz
    auto opts = reelay::discrete_timed<intmax_t>::robustness<double>::monitor<
                    input_t, output_t>::options()
                    .with_time_field_name(args::get(t_name))
                    .with_value_field_name(args::get(y_name))
                    .with_condensing(not args::get(fno_condense));

    monitor = reelay::make_monitor(args::get(spec), opts);

  } else if (use_dense and use_boolean and use_integer) {  // -vbi
    auto opts = reelay::dense_timed<intmax_t>::monitor<
                    input_t, output_t>::options()
                    .with_time_field_name(args::get(t_name))
                    .with_value_field_name(args::get(y_name));

    monitor = reelay::make_monitor(args::get(spec), opts);
  } else if (
      use_dense and use_boolean and use_floating and use_constant) {  // -vbf
                                                                      // -vbfk
    auto opts
        = reelay::dense_timed<double>::monitor<input_t, output_t>::options()
              .with_time_field_name(args::get(t_name))
              .with_value_field_name(args::get(y_name))
              .with_interpolation(reelay::piecewise::constant);

    monitor = reelay::make_monitor(args::get(spec), opts);

  } else if (
      use_dense and use_boolean and use_floating and use_linear) {  // -vbfl
    auto opts
        = reelay::dense_timed<double>::monitor<input_t, output_t>::options()
              .with_time_field_name(args::get(t_name))
              .with_value_field_name(args::get(y_name))
              .with_interpolation(reelay::piecewise::linear);

    monitor = reelay::make_monitor(args::get(spec), opts);

  } else if (use_dense and use_robustness and use_integer) {  // -vri
    auto opts = reelay::dense_timed<intmax_t>::robustness<double>::monitor<
                    input_t, output_t>::options()
                    .with_time_field_name(args::get(t_name))
                    .with_value_field_name(args::get(y_name));

    monitor = reelay::make_monitor(args::get(spec), opts);
  } else if (use_dense and use_robustness and use_floating) {  // -vrf
    auto opts = reelay::dense_timed<double>::robustness<double>::monitor<
                    input_t, output_t>::options()
                    .with_time_field_name(args::get(t_name))
                    .with_value_field_name(args::get(y_name));

    monitor = reelay::make_monitor(args::get(spec), opts);
  } else {
    throw std::invalid_argument("Unsupported flag combination");
  }

  if (use_discrete) {
    for (const auto& filename : filenames) {
      rycli::discrete_timed_processing(monitor, filename);
    }
  } else {
    for (const auto& filename : filenames) {
      rycli::dense_timed_processing(monitor, filename);
    }
  }

  return 0;
}

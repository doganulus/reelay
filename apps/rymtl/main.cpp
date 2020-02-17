/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "any"
#include "array"
#include "functional"
#include "iostream"
#include "memory"

#include "reelay/monitors.hpp"
#include "reelay/conversions.hpp"
#include "reelay/targets/stdout/formatter.hpp"

#include "../third_party/argparse.hpp"
#include "../third_party/csvparser_modern.hpp"

template <> struct reelay::field_of<csv::CSVRow> {
  static const std::unordered_set<std::string> falsity;
  inline static bool as_bool(const csv::CSVRow &args, const std::string &key) {
    return args.at(key) != "0";
  }
  inline static double as_float(const csv::CSVRow &args,
                                const std::string &key) {
    return std::stod(args.at(key));
  }
  inline static std::string as_string(const csv::CSVRow &args,
                                      const std::string &key) {
    return args.at(key);
  }
};

int main(int argc, const char *argv[]) {
  using time_t = int64_t;
  using input_t = csv::CSVRow;
  using interval_set = reelay::interval_set<time_t>;

  argparse::ArgumentParser program("rymtl");

  program.add_argument("spec").help(
      "specify a past metric temporal logic formula");

  program.add_argument("filename").help("specify a CSV file to monitor");

  /*  Verbosity
   *  ---------
   *  2 - Print inputs/output # NotImplementedYet
   *  1 - Print output only
   *  0 - Print violations only
   */
  program.add_argument("--verbose")
      .help("enable output verbosity level (default: 0)")
      .default_value(0)
      .action([](const std::string& value) { return std::stoi(value); });

  program.add_argument("--discrete")
      .help("enable discrete time semantics")
      .default_value(false)
      .implicit_value(true);

  program.add_argument("-p", "--period")
      .help("define a unit time period")
      .default_value(0)
      .action([](const std::string& value) { return std::stoi(value); });

  try {
    program.parse_args(argc, argv);
  } catch (const std::runtime_error& err) {
    std::cout << err.what() << std::endl;
    program.print_help();
    exit(0);
  }

  std::string spec = program.get<std::string>("spec");
  std::string filename = program.get<std::string>("filename");

  int verbosity = program.get<int>("--verbose");
  time_t period = program.get<int>("--period");

  csv::CSVReader reader(filename);

  int time;
  auto header = reader.get_col_names();

  if (program.get<bool>("--discrete")) {

    auto network =
        reelay::discrete_timed<time_t>::monitor<input_t>::from_temporal_logic(
            spec);

    auto formatter =
        reelay::discrete_timed_setting::make_stdout_formatter<input_t, time_t>(
            network, header, verbosity);

    std::cout << formatter->header();
    for (auto &row : reader) {
      network->update(row);
      std::cout << formatter->output();
    }

  } else if (period > 0) {
    auto network =
        reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic(
            spec);
    auto formatter =
        reelay::dense_timed_setting::make_stdout_formatter<input_t, time_t>(
            network, header, verbosity);

    std::cout << formatter->header();

    time_t now = 0;
    for (auto &row : reader) {
      now = now + period;
      network->update(row, now);
      std::cout << formatter->output();
    }

  } else {
    auto network =
        reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic(
            spec);
    auto formatter =
        reelay::dense_timed_setting::make_stdout_formatter<input_t, time_t>(
            network, header, verbosity);

    std::cout << formatter->header();

    for (auto &row : reader) {
      network->update(row);
      std::cout << formatter->output();
    }
  }
}


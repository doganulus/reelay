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
#include "fstream"
#include "memory"

#include "boost/algorithm/string.hpp"

#include "reelay/monitors.hpp"
#include "reelay/targets/stdout/formatter.hpp"


#include "../third_party/argparse.hpp"

    int
    main(int argc, const char *argv[]) {
  using time_t = int64_t;
  using input_t = std::vector<std::string>;

  argparse::ArgumentParser program("ryjavu");

  program.add_argument("spec").help(
      "specify a past quantified temporal logic formula");

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
      .default_value(true)
      .implicit_value(false);

  program.add_argument("--events")
      .help("enable discrete time semantics")
      .default_value(true)
      .implicit_value(false);

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

  std::ifstream input_stream;
  input_stream.open(filename);

  int verbosity = program.get<int>("--verbose");

  auto manager = std::make_shared<reelay::binding_manager>();
  reelay::kwargs extra_args = {{"manager", manager}};

  auto network =
      reelay::unordered_data::monitor<input_t>::from_temporal_logic(
          spec, extra_args);

  std::string row;
  while (std::getline(input_stream, row)) {
    std::vector<std::string> cells;
    boost::split(cells, row, [](char c){return c == ',';});
    network->update(cells);

    auto result = network->output();
    if(result != manager->one()){
      std::cout << network->now() << std::endl;
    }
    // if(network->now() >= 0 and network->now() <= 10){
    //   std::cout << result << std::endl;
    // }
  }
}


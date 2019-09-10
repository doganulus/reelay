#include "any"
#include "array"
#include "functional"
#include "iostream"
#include "memory"

#include "reelay/monitors.hpp"
#include "reelay/targets/stdout/formatter.hpp"

#include "../utils/argparse.hpp"
#include "../utils/csvstream.hpp"


int main(int argc, const char* argv[]) {
  using time_t = int64_t;
  using input_t = std::map<std::string, std::string>;
  using interval_set = reelay::interval_set<time_t>;

  argparse::ArgumentParser program("rymtl");

  program.add_argument("spec").help(
      "specify a past metric temporal logic formula");

  program.add_argument("filename").help("specify a CSV file to monitor");

  /*  Verbosity
   *  ---------
   *  2 - Print inputs/output
   *  1 - Print output only
   *  0 - Print violations only
   */
  program.add_argument("--verbose")
      .help("enable verbose printing (default: 2)")
      .default_value(2)
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

  csvstream csvin(filename);
  auto header = csvin.getheader();

  input_t row;

  if (program.get<bool>("--discrete")) {
  } else if (period > 0) {
    auto network =
      reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic(
            spec);
    auto formatter = 
      reelay::dense_timed_setting::make_stdout_formatter<input_t, time_t>(network, header, verbosity);

    std::cout << formatter->header();

    time_t now = 0;
    while (csvin >> row) {
      now = now + period;
      network->update(row, now);
      std::cout << formatter->output();
    }
  } else {

    if (std::find(header.begin(), header.end(), "time") != header.end()) {
      // Pass
    } else {
      throw std::runtime_error("No timestamps in the CSV file");
    }

    auto network =
      reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic(
            spec);
    auto formatter =
        reelay::dense_timed_setting::make_stdout_formatter<input_t, time_t>(
            network, header, verbosity);

    std::cout << formatter->header();

    while (csvin >> row) {
      network->update(row);
      std::cout << formatter->output();
    }
  }

  return 0;
}

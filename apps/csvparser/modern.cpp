#include "apps/utils/argparse.hpp"
#include "apps/utils/csvparser_modern.hpp"

int main(int argc, const char* argv[]) {

  argparse::ArgumentParser program("rymtl");

  program.add_argument("filename").help("specify a CSV file to monitor");

  try {
    program.parse_args(argc, argv);
  } catch (const std::runtime_error& err) {
    std::cout << err.what() << std::endl;
    program.print_help();
    exit(0);
  }

  std::string filename = program.get<std::string>("filename");

  int time;
  csv::CSVReader reader(filename);
  
  for (auto& row: reader) {
    time = std::stoi(row.at(0));
  }

  return 0;
}

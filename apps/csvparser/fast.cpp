#include "apps/utils/argparse.hpp"
#include "apps/utils/csvparser_fast.hpp"

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

  int t,p,q,r;
  io::CSVReader<4> reader(filename);
  reader.read_header(io::ignore_extra_column, "time", "p", "q", "r");

  std::array<int, 4> row;
  row[0] = 0;
  row[1] = 0;
  row[2] = 0;
  row[3] = 0;

  while(reader.read_row(t,p,q,r)){
    row[0] = t;
    row[1] = p;
    row[2] = q;
    row[3] = r;
  }

  return 0;
}

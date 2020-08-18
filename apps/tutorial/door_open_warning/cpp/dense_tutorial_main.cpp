#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "reelay/monitors.hpp"

int main(int argc, const char *argv[]) {

  using time_type = double;
  using input_type = reelay::json;
  using output_type = reelay::json;

  if (argc != 2) {
    std::cout << "Error: Program requires a filename argument." << std::endl;
    return -1;
  }

  std::ifstream input_file(argv[1]);

  // Monitor constructions
  auto options = reelay::dense_timed<time_t>::monitor<
      input_type, output_type>::options();

  auto my_monitor_1 = reelay::make_monitor(
      "(historically[0:5]{door_open} and not {dow_suppressed}) -> "
      "{door_open_warning}", options);
  auto my_monitor_2 = reelay::make_monitor(
      "{door_open_warning} -> historically[0:5]{door_open}", options);
  auto my_monitor_3 = reelay::make_monitor(
      "{door_open_warning} -> not {dow_suppressed}", options);
  auto my_monitor_4 = reelay::make_monitor(
      "{door_open_warning} -> not(once[1:1]({door_open} since {door_open_warning}))",
      options);

  auto ar1 = reelay::json::array();
  auto ar2 = reelay::json::array();
  auto ar3 = reelay::json::array();
  auto ar4 = reelay::json::array();

  // Execution
  for (std::string line; std::getline(input_file, line);) {

    reelay::json message = reelay::json::parse(line);

    auto r1 = my_monitor_1.update(message);
    auto r2 = my_monitor_2.update(message);
    auto r3 = my_monitor_3.update(message);
    auto r4 = my_monitor_4.update(message);

    if(not r1.empty()){
      ar1.insert(ar1.end(), r1.begin(), r1.end());
    }
    if (not r2.empty()) {
      ar2.insert(ar2.end(), r2.begin(), r2.end());
    }
    if (not r3.empty()) {
      ar3.insert(ar3.end(), r3.begin(), r3.end());
    }
    if (not r4.empty()) {
      ar4.insert(ar4.end(), r4.begin(), r4.end());
    }
  }

  std::cout << "SYS-REQ-1 Behavior:" << std::endl;
  std::cout << ar1 << std::endl;

  std::cout << "SYS-REQ-2 Behavior:" << std::endl;
  std::cout << ar2 << std::endl;

  std::cout << "SYS-REQ-3 Behavior:" << std::endl;
  std::cout << ar3 << std::endl;

  std::cout << "SYS-REQ-4 Behavior:" << std::endl;
  std::cout << ar4 << std::endl;

  std::cout << "Exited successfully." << std::endl;

  return 0;
}
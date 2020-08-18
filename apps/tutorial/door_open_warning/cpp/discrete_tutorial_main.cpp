#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "reelay/monitors.hpp"

int main(int argc, const char *argv[]) {

  using time_type = int64_t;
  using input_type = reelay::json;
  using output_type = reelay::json;

  if (argc != 2) {
    std::cout << "Error: Program requires a filename argument." << std::endl;
    return -1;
  }

  std::ifstream input_file(argv[1]);

  // Monitor constructions
  auto options = reelay::discrete_timed<time_t>::monitor<
      input_type, output_type>::options().disable_condensing();

  auto my_monitor_1 = reelay::make_monitor(
      "(historically[0:5]{door_open} and not {dow_suppressed}) -> "
      "{door_open_warning}", options);
  auto my_monitor_2 = reelay::make_monitor(
      "{door_open_warning} -> historically[0:5]{door_open}", options);
  auto my_monitor_3 = reelay::make_monitor(
      "{door_open_warning} -> not {dow_suppressed}", options);
  auto my_monitor_4 = reelay::make_monitor(
      "{door_open_warning} -> not(pre({door_open} since {door_open_warning}))",
      options);

  // Execution
  for (std::string line; std::getline(input_file, line);) {

    reelay::json message = reelay::json::parse(line);

    auto r1 = my_monitor_1.update(message);
    auto r2 = my_monitor_2.update(message);
    auto r3 = my_monitor_3.update(message);
    auto r4 = my_monitor_4.update(message);

    if (r1["value"] == false) {
      std::cout << "Error at " << my_monitor_1.now()
                << " : False negative detected (SYS-REQ-01 Violation)"
                << std::endl;
    }
    if (r2["value"] == false) {
      std::cout << "Error at " << my_monitor_2.now()
                << " : False positive detected (SYS-REQ-01 Violation)"
                << std::endl;
    }
    if (r3["value"] == false) {
      std::cout << "Error at " << my_monitor_3.now()
                << " : False positive detected (SYS-REQ-01 Violation)"
                << std::endl;
    }
    if (r4["value"] == false) {
      std::cout << "Error at " << my_monitor_4.now()
                << " : False positive detected (SYS-REQ-02 Violation)"
                << std::endl;
    }
  }

  std::cout << "Exited successfully." << std::endl;

  return 0;
}
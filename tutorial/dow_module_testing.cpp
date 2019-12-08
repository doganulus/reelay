#include "string"
#include "vector"
#include "map"
#include "iostream"
#include "memory"

#include "reelay/recipes.hpp"

int main(int argc, const char *argv[]) {

  using time_t = int64_t;
  using input_t = std::map<std::string, time_t>;

  std::vector<input_t> faulty_sys_behavior = std::vector<input_t>();

  faulty_sys_behavior.push_back(input_t{{"time", 1},
                                        {"door_open", 0},
                                        {"dow_suppressed", 0},
                                        {"door_open_warning", 0}});
  faulty_sys_behavior.push_back(input_t{{"time", 2},
                                        {"door_open", 1},
                                        {"dow_suppressed", 0},
                                        {"door_open_warning", 0}});
  faulty_sys_behavior.push_back(input_t{{"time", 3},
                                        {"door_open", 1},
                                        {"dow_suppressed", 0},
                                        {"door_open_warning", 0}});
  faulty_sys_behavior.push_back(input_t{{"time", 4},
                                        {"door_open", 1},
                                        {"dow_suppressed", 0},
                                        {"door_open_warning", 0}});
  faulty_sys_behavior.push_back(input_t{{"time", 5},
                                        {"door_open", 1},
                                        {"dow_suppressed", 0},
                                        {"door_open_warning", 0}});
  faulty_sys_behavior.push_back(input_t{{"time", 6},
                                        {"door_open", 1},
                                        {"dow_suppressed", 0},
                                        {"door_open_warning", 0}});
  faulty_sys_behavior.push_back(input_t{{"time", 7},
                                        {"door_open", 1},
                                        {"dow_suppressed", 0},
                                        {"door_open_warning", 1}});
  faulty_sys_behavior.push_back(input_t{{"time", 8},
                                        {"door_open", 1},
                                        {"dow_suppressed", 1},
                                        {"door_open_warning", 0}});
  faulty_sys_behavior.push_back(input_t{{"time", 9},
                                        {"door_open", 1},
                                        {"dow_suppressed", 1},
                                        {"door_open_warning", 1}});

  auto my_monitor_1 = reelay::discrete_timed_past_mtl_monitor(
      "(historically[0:5](door_open) and not dow_suppressed) -> "
      "door_open_warning");
  auto my_monitor_2 = reelay::discrete_timed_past_mtl_monitor(
      "door_open_warning -> historically[0:5](door_open)");
  auto my_monitor_3 = reelay::discrete_timed_past_mtl_monitor(
      "door_open_warning -> not dow_suppressed");
  auto my_monitor_4 = reelay::discrete_timed_past_mtl_monitor(
      "door_open_warning -> not(pre(door_open since door_open_warning))");

  for (const auto &message : faulty_sys_behavior) {

    auto check_1 = my_monitor_1.update(message);
    auto check_2 = my_monitor_2.update(message);
    auto check_3 = my_monitor_3.update(message);
    auto check_4 = my_monitor_4.update(message);

    if (not check_1) {
      std::cout << "Error at time " << my_monitor_1.now()
                << " : False negative detected (SYS-REQ-01 Violation)"
                << std::endl;
    }
    if (not check_2) {
      std::cout << "Error at time " << my_monitor_2.now()
                << " : False positive detected (SYS-REQ-01 Violation)"
                << std::endl;
    }
    if (not check_3) {
      std::cout << "Error at time " << my_monitor_3.now()
                << " : False positive detected (SYS-REQ-01 Violation)"
                << std::endl;
    }
    if (not check_4) {
      std::cout << "Error at time " << my_monitor_4.now()
                << " : False positive detected (SYS-REQ-02 Violation)"
                << std::endl;
    }
  }

  return 0;
}
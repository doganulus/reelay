# Getting Started From C++

In this part, we show how to instatiate and execute **Reelay** monitors in C++ given formal specifications. These monitors would observe the system behavior and report violations at runtime. First recall our specifications for the Door Open Warning (DOW) feature of a home assistant robot as explained in [the introduction](gs_intro.md).

The source code of this tutorial can be found [here](https://github.com/doganulus/reelay/blob/master/tutorial/dow_module_testing.cpp).

## Reelay C++ Library and Recipes

The core component of Reelay is a header-only template library for C++ supporting several specification languages. Templates allow to customize monitors for different input types (e.g `std::tuple` or `std::map`) or different time types (e.g. `int64_t` or `double`). In this tutorial, we will use **Reelay** recipes, a predefined selection of concrete monitor classes that instatiates monitor templates by convenient datatypes. We include **Reelay** recipes into our source file as follows:

    #include "reelay/recipes.hpp"

Some of these recipes are given below in the table together with predefined datatypes. Main differences between these monitor classes are the time model, either `discrete` or `dense` (roughly meaning-- might have time gaps between samples), and the input type whether we monitor Boolean signals (as in `past_ltl_monitor`) or numerical signals (as in `past_stl_monitor`).

| Class Name | `input_t` | `time_t` | Gaps in behaviors |
|-|-|-|-|
|`reelay::past_ltl_monitor` | `std::map<std::string, bool>`|-| No |
|`reelay::past_mtl_monitor` | `std::map<std::string, int64_t>` | `int64_t` | **Yes** |
|`reelay::discrete_timed_past_mtl_monitor` | `std::map<std::string, int64_t>` | `int64_t` | No |
|`reelay::past_stl_monitor` | `std::map<std::string, double>` | `double` | **Yes**|
|`reelay::discrete_timed_past_stl_monitor` | `std::map<std::string, double>` | `int64_t` | No |

Please also check [the User Manual](user_manual.md) for the information regarding `reelay/recipes.hpp` as well as `reelay/monitors.hpp`, which provide a nice interface for full datatype customization of monitors. We provide further information on [time models](time_models.md) and [temporal logics](temporal_logic.md) under advanced topics.

## Check Requirements over System Behaviors

Let's put correct system behavior into a container for demo purposes. Later we will read one-by-one it as if it comes from a real-time system.

```cpp
using input_t = std::map<std::string, int64_t>;
std::vector<input_t> correct_sys_behavior = std::vector<input_t>();

correct_sys_behavior.push_back( 
	input_t{{"time", 1}, {"door_open", 0}, {"dow_suppressed", 0}, {"door_open_warning", 0}} );
correct_sys_behavior.push_back( 
	input_t{{"time", 2}, {"door_open", 1}, {"dow_suppressed", 0}, {"door_open_warning", 0}} );
correct_sys_behavior.push_back( 
	input_t{{"time", 3}, {"door_open", 1}, {"dow_suppressed", 0}, {"door_open_warning", 0}} );
correct_sys_behavior.push_back( 
	input_t{{"time", 4}, {"door_open", 1}, {"dow_suppressed", 0}, {"door_open_warning", 0}} );
correct_sys_behavior.push_back( 
	input_t{{"time", 5}, {"door_open", 1}, {"dow_suppressed", 0}, {"door_open_warning", 0}} );
correct_sys_behavior.push_back( 
	input_t{{"time", 6}, {"door_open", 1}, {"dow_suppressed", 0}, {"door_open_warning", 0}} );
correct_sys_behavior.push_back( 
	input_t{{"time", 7}, {"door_open", 1}, {"dow_suppressed", 0}, {"door_open_warning", 1}} );
correct_sys_behavior.push_back( 
	input_t{{"time", 8}, {"door_open", 1}, {"dow_suppressed", 1}, {"door_open_warning", 0}} );
correct_sys_behavior.push_back( 
	input_t{{"time", 9}, {"door_open", 1}, {"dow_suppressed", 1}, {"door_open_warning", 0}} );
```

The use of `reelay` monitors is pretty straighforward and we construct an online monitor for our first specification as follows:

```cpp
auto my_monitor_1 = reelay::discrete_timed_past_mtl_monitor(
	"(historically[0:5]{door_open} and not{dow_suppressed}) ->
	   {door_open_warning}");
```

The rest of the program reads the system behavior incrementally and feeds the monitor accordingly.

```cpp
for (const auto &message : correct_sys_behavior)
{
	auto check_1 = my_monitor_1.update(message);

	if (not check_1) {
	std::cout << "Error at time " << my_monitor_1.now()
			  << " : False negative detected (SYS-REQ-01 Violation)"
			  << std::endl;
	}
}
```

The monitor would return `false` if it detects a violation then the program print an error message. When checking `correct_sys_behavior`, we will not see any error as the behavior satisfies the requirement. In the next section, however, we will insert some errors into the behavior and **Reelay** monitors would catch them all.

## Inserting an Error

(Under construction)

## Time Gaps in Behaviors

(Under construction)

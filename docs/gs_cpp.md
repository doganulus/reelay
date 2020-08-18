# Getting Started From C++

In this part, we show how to instatiate and run **Reelay** monitors in C++ given formal specifications. These monitors would check the system behavior and report violations at runtime. If you have't already, please recall our specifications for the Door Open Warning (DOW) feature of a home assistant robot as explained in [the introduction](gs_intro.md).

Source files for this tutorial can be found [here](https://github.com/doganulus/reelay/blob/master/apps/tutorial/door_open_warning).

## Reelay Monitors

The core component of Reelay is a header-only template library for C++ and can be included as follows. 

```cpp
#include "reelay/monitors.hpp"
```

In this tutorial we use `options` and `make_monitor` constructs to configure and make runtime monitors from our specifications. The library gives you some options to configure monitors. The first and most important configuration option is the time model where you select one

1. Discrete timed model
2. Dense timed model

Normally you would select discrete timed model if you work on discrete time behaviors and dense timed model if dense time behaviors. This choice depends on on your system and needs. If you haven't already, please check the documentation on [discrete and dense time behaviors](docs/behaviors.md) before proceeding.

## Check Requirements over Discrete Time Behaviors

In this section we demonstrate how to configure, build and run a discrete timed Reelay monitor. First, we create an `options` object as follows:

```cpp
auto opts = reelay::discrete_timed<time_type>
	::monitor<input_t, output_t>::options()
		.disable_condensing()
		.with_value_field_name("verdict");
```

Let's explain these options. As mentioned earlier, the first options says that we want a discrete timed monitor and we use `time_type` as the Since this is a discrete monitor, `time_type` must an integral number type, typically `intmax_t`.

The second part sets `input_t` and `output_t` types of input and output types of our monitors. By default, set these types to `reelay::json` (alias to `nlohmann::json`). For more advanced uses, the library allows you to use other (map-like) types if you provide some adapter/formatter classes. But, for now, use `reelay::json`. The remaining options controls the output of the monitor, which we will explain shortly.

Once we have our `opts` object, we make our monitor from a specification as follows:

```cpp
std::string pattern = "(historically[0:5]{door_open} and not {dow_suppressed}) -> {door_open_warning}";
auto my_monitor_1 = reelay::make_monitor(pattern, opts);
```

Note that you can use the same `opts` to make different monitors with the same configuration. THen we can execute each monitor using `update` methods that accepts a single `input_t` objects. For example, if we are reading a JSON Lines logfile (opened as `input_file`), we check the logfile line-by-line as follows: 

```cpp
for (std::string line; std::getline(input_file, line);) {

  reelay::json message = reelay::json::parse(line);
  auto result1 = my_monitor_1.update(message);

  if (result1["verdict"] == false) {
      std::cout << "Error at " << my_monitor_1.now()
                << " : False negative detected (SYS-REQ-01 Violation)"
                << std::endl;
  }

}
```

The output object `result1` is a `output_t` object (here reelay::json) and `verdict` field (as set in `opts`) would contain the monitor result at each time. Finally in this tutorial, since our requirements must be `true` for all time points, we print an error message if there is any violation.

The complete source file of the discrete timed monitor of DOW warning tutorial can be found [here](https://github.com/doganulus/reelay/blob/master/apps/tutorial/door_open_warning/cpp/discrete_tutorial_main.cpp).

## Check Requirements over Dense Time Behaviors

In this section we demonstrate how to configure, build and run a dense timed Reelay monitor. Similar to the discrete monitors, we create an `options` object as follows:

```cpp
auto opts = reelay::dense_timed<time_type>
	::monitor<input_t, output_t>::options()
		.with_time_field_name("timestamp")
		.with_value_field_name("verdict");
```

Here we used `dense_timed<time_type>` construct to denote we want a dense timed monitor. Unlike discrete time monitors, here we can use floating types, typically `double`. After we set out input-output types, we make our dense timed monitors in the same way:

```cpp
std::string pattern = "(historically[0:5]{door_open} and not {dow_suppressed}) -> {door_open_warning}";
auto my_monitor_1 = reelay::make_monitor(pattern, opts);
```
The execution also works similar way; however, now notice that the output is a `json::array` in particular. This is due to the verdict can change multiple times between two updates as we progress in time at arbitrary amounts in dense timed model. 
	In other words, our monitors outputs all changes between the last time and current time point. And if nothing changes the outputs would be just empty. Below is an example execution for dense timed monitors.  

```cpp
for (std::string line; std::getline(input_file, line);) {

  reelay::json message = reelay::json::parse(line);
  auto r1 = my_monitor_1.update(message);

  if (not r1.empty()){
    for (const auto& segment : r1) {
      std::cout << segment << std::endl;
    }
  }

}
```

The complete source file of the dense timed monitor of DOW warning tutorial can be found [here](https://github.com/doganulus/reelay/blob/master/apps/tutorial/door_open_warning/cpp/dense_tutorial_main.cpp).
# Getting Started From C++

`Reelay` has been designed for efficiency and flexibility so that users can write their applications in C++ or embed formal specification monitoring into their projects easily. 

    #include "reelay/recipes.hpp"

The use of `reelay` monitors is pretty straighforward and we construct an online monitor for a past temporal logic formula as follows: 

	auto my_monitor = reelay::past_ltl_monitor("grant -> once(request)");

	for (const auto &current_input : input_sequence)
	{
		my_monitor->update(current_input);     // feed with current input
		current_output = my_monitor->output(); // obtain the current output
	}

Since this is a property that should be true at each and every time point 

The ability to construct discrete and dense timed monitors from timed specifications is the main goal and feature of `reelay` project. We construct online monitor from such specification as follows:
     
    auto my_monitor = reelay::discrete_timed<time_t>::monitor<input_t>::from_temporal_logic("p1 since[18:24] p2");

    auto my_monitor = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("p1 since[18:24] p2");

From the online monitoring point of view, the essential difference between discrete and dense time models is the duration of updates, which is strictly one time unit for the discrete setting whereas it could be arbitrarily long or short for the dense setting. For more information, please check [discrete]() and [dense]() timed settings. These settings are usually called metric temporal logic (MTL) and very popular for specifiying properties of real-time/cyber-physical systems.


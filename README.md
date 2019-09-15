# Reelay

`Reelay` is a user-friendly C++ library for online monitoring of formal specifications such as past temporal logic and regular expressions. Please see install instructions [here](https://github.com/doganulus/reelay/blob/master/INSTALL.md).

# Brief
`Reelay` has been designed for efficiency and flexibility so that users can write their applications in C++ or embed formal specification monitoring into their projects easily. 

The use of `reelay` monitors is pretty straighforward and we construct an online monitor for a past temporal logic formula as follows: 

	auto my_monitor = reelay::monitor<input_t>::from_temporal_logic("p1 since p2");

	for (const auto &current_input : input_sequence)
	{
		my_monitor->update(current_input);     // feed with current input
		current_output = my_monitor->output(); // obtain the current output
	}

The ability to construct discrete and dense timed monitors from timed specifications is the main goal and feature of `reelay` project. We construct online monitor from such specification as follows:
     
    auto my_monitor = reelay::discrete_timed<time_t>::monitor<input_t>::from_temporal_logic("p1 since[18:24] p2");

    auto my_monitor = reelay::dense_timed<time_t>::monitor<input_t>::from_temporal_logic("p1 since[18:24] p2");

From the online monitoring point of view, the essential difference between discrete and dense time models is the duration of updates, which is strictly one time unit for the discrete setting whereas it could be arbitrarily long or short for the denseW setting. For more information, please check [discrete]() and [dense]() timed settings. These settings are usually called metric temporal logic (MTL) and very popular for specifiying properties of real-time/cyber-physical systems.

# Monitoring Apps

A set of (standalone, command-line) online monitoring applications utilizing `reelay` library over some common data formats such as comma separated values (CSV) files. The apps folder would contain the source code these applications and can be built using `make apps` command. Please check for more information regarding stand-alone monitoring apps.

The first application `rymtl` monitors  over CSV files providing a number of convinient features such as dense-time monitoring by fixed duration steps.

# Current Roadmap

`reelay` is an ongoing project and below is a number of features and plans to expand the scope and improve the usability of the library.

| Status | Version | Deliverable | Codename |
|-|---------|-------------|----------|
|✅| v.1.1 | Untimed, discrete and dense timed settings for past temporal logic | MTL
|✅| v.1.2 | MTL monitoring application | rymtl
|⌛| v.1.3 | Full predicate support over piecewise linear signals | STL
|⌛| v.1.4 | Robustness settings | STLRO
|⌛| v.1.5 | STL monitoring application | rystl
|🤞| v.1.6 | Python bindings | MONPY
|🤞| v.1.x | Untimed regular expressions | RE
|🤞| v.1.x | Discrete and dense timed regular expressions | TRE
|🤞| v.1.x | TRE monitoring application | rytre
|🤞| v.1.x | First-order quantification for untimed past temporal logic | FOLTL
|🤞| v.1.x | First-order quantification for past metric temporal logic | FOMTL
|🤞| v.1.x | FOL monitoring application | ryjavu |
|🤞| v.2.0 | Reelay 2.0! | REELAY
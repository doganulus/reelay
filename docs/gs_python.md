# Getting Started From Python

In this part, we show how to instatiate and execute **Reelay** monitors from Python given formal specifications. These monitors would observe the system behavior and report violations at runtime. First recall our specifications for the Door Open Warning (DOW) feature of a home assistant robot as explained in [the introduction](gs_intro.md).

The source code of this tutorial can be found [here](https://github.com/doganulus/reelay/blob/master/tutorial/dow_module_testing.py).

## Reelay Python Package

The core component of Reelay is a header-only template library for C++ supporting several specification languages. In [C++ tutorial](gs_cpp.md), we introduced **Reelay** monitors that instantiate template classes with concrete datatypes. **Reelay** Python package is essentially Python bindings of these monitors with pre-defined datatypes and a thin layer of Pythonic user interface. 

We import Reelay monitors as follows:
```python
import reelay
```

The package consists of two constructs `discrete_time_monitor` and `dense_timed_monitor`. Normally you would select discrete timed model if you work on discrete time behaviors and dense timed model if dense time behaviors. This choice depends on on your system and needs. If you haven't already, please check the documentation on [discrete and dense time behaviors](docs/behaviors.md) before proceeding.

## Check Requirements over Discrete Time Behaviors

In this section we demonstrate how to configure, build and run a discrete timed Reelay monitor in Python. As typical in Python, we give monitor configuration options as keyword arguments as follows.

```python
my_monitor_1 = reelay.discrete_timed_monitor(
    pattern=r"(historically[0:5]{door_open} and not {dow_suppressed}) -> {door_open_warning}", condense=False)
```

Reelay Python monitor uses Python dicts/lists conveniently. Then we execute the monitor using `update` method, which accepts a single Python dictionary.

```python
for x in faulty_sys_behavior: 
    r1 = my_monitor_1.update(x)
    if r1["value"] is False:
        print('Error at {err_time} : False negative detected (SYS-REQ-01 Violation)'.format(err_time=my_monitor_1.now()))
```

where `faulty_sys_behavior` is a list of Python dicts. And the output `r1` is similarly a Python dict that contains the result at that time step.  Finally in this tutorial, since our requirements must be `true` for all time points, we print an error message if there is any violation.

The complete source file of the discrete timed monitor of DOW warning tutorial can be found [here](https://github.com/doganulus/reelay/blob/master/apps/tutorial/door_open_warning/python/discrete_tutorial.py).

## Check Requirements over Dense Time Behaviors

In this section we demonstrate how to configure, build and run a dense timed Reelay monitor in Python. Similar to the discrete monitors, we create a dense timed monitor as folows:

```python
my_monitor_1 = reelay.dense_timed_monitor(
    pattern=r"(historically[0:5]{door_open} and not {dow_suppressed}) -> {door_open_warning}")
```

 For dense timed monitors, remember that we yield a list of dicts at each update step. This is due to the verdict can change multiple times between two updates as we progress in time at arbitrary amounts in dense timed model. In other words, our monitors outputs all changes between the last time and current time point. And if nothing changes the outputs would be just empty. Below is an example execution for dense timed monitors.  

```python
results = []
for x in faulty_sys_behavior: 
    r1 = my_monitor_1.update(x)
    results.extend(r1)

print(results) # Print all
```

where `faulty_sys_behavior` is a list of dicts. And the output `r1` is similarly a list of dicts that contains the result at that time step.

The complete source file of the dense timed monitor of DOW warning tutorial can be found [here](https://github.com/doganulus/reelay/blob/master/apps/tutorial/door_open_warning/python/dense_tutorial.py).
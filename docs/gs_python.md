# Getting Started From Python

!!! Warning
    Reelay Python API is in its early stage. Everthing in this page is subject to change.

In this part, we show how to instatiate and execute **Reelay** monitors from Python given formal specifications. These monitors would observe the system behavior and report violations at runtime. First recall our specifications for the Door Open Warning (DOW) feature of a home assistant robot as explained in [the introduction](gs_intro.md).

The source code of this tutorial can be found [here](https://github.com/doganulus/reelay/blob/master/tutorial/dow_module_testing.py).

## Reelay Python Package

The core component of Reelay is a header-only template library for C++ supporting several specification languages. In [C++ tutorial](gs_cpp.md), we introduced **Reelay** recipes that instantiate template classes with concrete datatypes. **Reelay** Python package is essentially Python bindings of these recipes together with a thin layer of Pythonic user interface. In this tutorial, we will use **Reelay** Python package imported into your project as follows:

```python
import reelay
```

Some of these recipes are given below in the table together with predefined datatypes. Main differences between these monitor classes are the time model, either `discrete` or `dense` (roughly meaning-- might have time gaps between samples), and the input type whether we monitor Boolean signals (as in `past_ltl_monitor`) or numerical signals (as in `past_stl_monitor`).

| Class Name | Expected input type | Default Time Model |
|-|-|-|
|`reelay.past_ltl.monitor` | `dict[str, bool]` | N/A |
|`reelay.past_mtl.monitor` | `dict[str, int]`  | `discrete` |
|`reelay.past_stl.monitor` | `dict[str, float]` | `dense` |

Please also check [the User Manual](user_manual.md) for the information regarding Python API. We provide further information on [time models](time_models.md) and [temporal logics](temporal_logic.md) under advanced topics.

## Check Requirements over System Behaviors

Let's put correct system behavior into a container for demo purposes. Later we will read one-by-one it as if it comes from a real-time system.

```python
correct_sys_behavior = [
    dict(time=1, door_open=False,dow_suppressed=False, door_open_warning=False),
    dict(time=2, door_open=True, dow_suppressed=False, door_open_warning=False),
    dict(time=3, door_open=True, dow_suppressed=False, door_open_warning=False),
    dict(time=4, door_open=True, dow_suppressed=False, door_open_warning=False),
    dict(time=5, door_open=True, dow_suppressed=False, door_open_warning=False),
    dict(time=6, door_open=True, dow_suppressed=False, door_open_warning=False),
    dict(time=7, door_open=True, dow_suppressed=False, door_open_warning=True),
    dict(time=8, door_open=True, dow_suppressed=True, door_open_warning=False),
    dict(time=9, door_open=True, dow_suppressed=True, door_open_warning=False),
    ]
```

The use of `reelay` monitors is pretty straighforward and we construct an online monitor for our first specification as follows:

```python
my_monitor_1 = reelay.past_mtl.monitor(
    pattern=r"(historically[0:5]{door_open} and not {dow_suppressed} -> "
      "door_open_warning",
    time_model="discrete")
```

The rest of the program reads the system behavior incrementally and feeds the monitor accordingly.

```python
for message in correct_sys_behavior:
    check_1 = my_monitor_1.update(message)
    if not check_1:
        print('Error at time {err_time} : False negative detected (SYS-REQ-01 Violation)'
            .format(err_time=my_monitor_1.now()))
```

The monitor would return `false` if it detects a violation then the program print an error message. When checking `correct_sys_behavior`, we will not see any error as the behavior satisfies the requirement. In the next section, however, we will insert some errors into the behavior and **Reelay** monitors would catch them all. 

## Inserting an Error

(Under construction)

## Time Gaps in Behaviors

(Under construction)
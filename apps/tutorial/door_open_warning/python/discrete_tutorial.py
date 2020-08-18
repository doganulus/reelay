import reelay

correct_sys_behavior = [
    dict(door_open=False, dow_suppressed=False, door_open_warning=False),
    dict(door_open=True, dow_suppressed=False, door_open_warning=False),
    dict(door_open=True, dow_suppressed=False, door_open_warning=False),
    dict(door_open=True, dow_suppressed=False, door_open_warning=False),
    dict(door_open=True, dow_suppressed=False, door_open_warning=False),
    dict(door_open=True, dow_suppressed=False, door_open_warning=False),
    dict(door_open=True, dow_suppressed=False, door_open_warning=True),
    dict(door_open=True, dow_suppressed=True, door_open_warning=False),
    dict(door_open=True, dow_suppressed=True, door_open_warning=False),
]


faulty_sys_behavior = [
    dict(door_open=False,dow_suppressed=False, door_open_warning=False),
    dict(door_open=True, dow_suppressed=False, door_open_warning=False),
    dict(door_open=True, dow_suppressed=False, door_open_warning=False),
    dict(door_open=True, dow_suppressed=False, door_open_warning=False),
    dict(door_open=True, dow_suppressed=False, door_open_warning=False),
    dict(door_open=True, dow_suppressed=False, door_open_warning=False),
    dict(door_open=True, dow_suppressed=False, door_open_warning=True),
    dict(door_open=True, dow_suppressed=True, door_open_warning=False),
    dict(door_open=True, dow_suppressed=True, door_open_warning=True),
    ]

my_monitor_1 = reelay.discrete_timed_monitor(
    pattern=r"(historically[0:5]{door_open} and not {dow_suppressed}) -> {door_open_warning}", condense=False)

my_monitor_2 = reelay.discrete_timed_monitor(
    pattern=r"{door_open_warning} -> historically[0:5]{door_open}", condense=False)

my_monitor_3 = reelay.discrete_timed_monitor(
    pattern=r"{door_open_warning} -> not {dow_suppressed}", condense=False)

my_monitor_4 = reelay.discrete_timed_monitor(
    pattern=r"{door_open_warning} -> not(pre({door_open} since {door_open_warning}))", condense=False)

for x in faulty_sys_behavior:  # Change to correct_sys_behavior
    r1 = my_monitor_1.update(x)
    r2 = my_monitor_2.update(x)
    r3 = my_monitor_3.update(x)
    r4 = my_monitor_4.update(x)

    if r1["value"] is False:
        print('Error at {err_time} : False negative detected (SYS-REQ-01 Violation)'.format(err_time=my_monitor_1.now()))
    if r2["value"] is False:
        print('Error at {err_time} : False positive detected (SYS-REQ-01 Violation)'.format(err_time=my_monitor_2.now()))
    if r3["value"] is False:
        print('Error at {err_time} : False positive detected (SYS-REQ-01 Violation)'.format(err_time=my_monitor_3.now()))
    if r4["value"] is False:
        print('Error at {err_time} : False positive detected (SYS-REQ-02 Violation)'.format(err_time=my_monitor_4.now()))
    

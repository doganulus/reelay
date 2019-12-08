import reelay

faulty_sys_behavior = [
    dict(time=1, door_open=False,dow_suppressed=False, door_open_warning=False),
    dict(time=2, door_open=True, dow_suppressed=False, door_open_warning=False),
    dict(time=3, door_open=True, dow_suppressed=False, door_open_warning=False),
    dict(time=4, door_open=True, dow_suppressed=False, door_open_warning=False),
    dict(time=5, door_open=True, dow_suppressed=False, door_open_warning=False),
    dict(time=6, door_open=True, dow_suppressed=False, door_open_warning=False),
    dict(time=7, door_open=True, dow_suppressed=False, door_open_warning=True),
    dict(time=8, door_open=True, dow_suppressed=True, door_open_warning=False),
    dict(time=9, door_open=True, dow_suppressed=True, door_open_warning=True),
    ]

my_monitor_1 = reelay.past_mtl.monitor(
    pattern="(historically[0:5](door_open) and not dow_suppressed) -> "
      "door_open_warning", 
    time_model="discrete")

my_monitor_2 = reelay.past_mtl.monitor(
    pattern="door_open_warning -> historically[0:5](door_open)", 
    time_model="discrete")

my_monitor_3 = reelay.past_mtl.monitor(
    pattern="door_open_warning -> not dow_suppressed", 
    time_model="discrete")

my_monitor_4 = reelay.past_mtl.monitor(
    pattern="door_open_warning -> not(pre(door_open since door_open_warning))", 
    time_model="discrete")

for x in faulty_sys_behavior:
    check_1 = my_monitor_1.update(x)
    check_2 = my_monitor_2.update(x)
    check_3 = my_monitor_3.update(x)
    check_4 = my_monitor_4.update(x)

    if not check_1:
        print('Error at time {err_time} : False negative detected (SYS-REQ-01 Violation)'.format(err_time=my_monitor_1.now()))
    if not check_2:
        print('Error at time {err_time} : False positive detected (SYS-REQ-01 Violation)'.format(err_time=my_monitor_2.now()))
    if not check_3:
        print('Error at time {err_time} : False positive detected (SYS-REQ-01 Violation)'.format(err_time=my_monitor_3.now()))
    if not check_4:
        print('Error at time {err_time} : False positive detected (SYS-REQ-02 Violation)'.format(err_time=my_monitor_4.now()))
    

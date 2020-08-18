import reelay 


correct_sys_behavior = [
    dict(time=0, door_open=False, dow_suppressed=False, door_open_warning=False),
    dict(time=1, door_open=True, dow_suppressed=False, door_open_warning=False),
    dict(time=6, door_open=True, dow_suppressed=False, door_open_warning=True),
    dict(time=7, door_open=True, dow_suppressed=True, door_open_warning=False),
    dict(time=8),
]


faulty_sys_behavior = [
    dict(time=0, door_open=False, dow_suppressed=False, door_open_warning=False),
    dict(time=1, door_open=True, dow_suppressed=False, door_open_warning=False),
    dict(time=6, door_open=True, dow_suppressed=False, door_open_warning=True),
    dict(time=7, door_open=True, dow_suppressed=True, door_open_warning=False),
    dict(time=8, door_open=True, dow_suppressed=True, door_open_warning=True),
    dict(time=9),
]

my_monitor_1 = reelay.dense_timed_monitor(
    pattern=r"(historically[0:5]{door_open} and not {dow_suppressed}) -> {door_open_warning}")

my_monitor_2 = reelay.dense_timed_monitor(
    pattern=r"{door_open_warning} -> historically[0:5]{door_open}")

my_monitor_3 = reelay.dense_timed_monitor(
    pattern=r"{door_open_warning} -> not {dow_suppressed}")

my_monitor_4 = reelay.dense_timed_monitor(
    pattern=r"{door_open_warning} -> not(once[1:1]({door_open} since {door_open_warning}))")

ar1 = []
ar2 = []
ar3 = []
ar4 = []

for x in faulty_sys_behavior:  # Change to correct_sys_behavior
    ar1.extend(my_monitor_1.update(x))
    ar2.extend(my_monitor_2.update(x))
    ar3.extend(my_monitor_3.update(x))
    ar4.extend(my_monitor_4.update(x))

print("SYS-REQ-1 behavior:")
print(ar1)
print("SYS-REQ-2 behavior:")
print(ar2)
print("SYS-REQ-3 behavior:")
print(ar3)
print("SYS-REQ-4 behavior:")
print(ar4)

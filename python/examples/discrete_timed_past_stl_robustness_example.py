import reelay

my_monitor = reelay.past_stl.monitor(
    pattern="historically[:4](speed > 13.0)", 
    time_model="discrete",
    enable_robustness=True
    )

input_sequence = [
    dict(speed=3.3),
    dict(speed=6.3),
    dict(speed=9.3),
    dict(speed=13.3),
    dict(speed=13.4),
    dict(speed=13.3),
    dict(speed=13.2),
    dict(speed=13.2),
    dict(speed=13.4),
    dict(speed=13.4),
    dict(speed=13.3),
    dict(speed=13.1),
    dict(speed=13.4),
    dict(speed=13.4),
    dict(speed=13.4),
    dict(speed=13.4),
    dict(speed=13.4),
    dict(speed=13.4),
    ]

output_sequence = []
for x in input_sequence:
    current_output = my_monitor.update(x)
    output_sequence.append(current_output)

print(output_sequence)
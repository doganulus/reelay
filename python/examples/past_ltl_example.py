import reelay

my_monitor = reelay.past_ltl.monitor(
    pattern="p1 since p2")

input_sequence = [
    dict(p1=False, p2=False),
    dict(p1=False, p2=True),
    dict(p1=True,  p2=False),
    dict(p1=True,  p2=False),
    dict(p1=True,  p2=False),
    dict(p1=True,  p2=False),
    dict(p1=True,  p2=False),
    dict(p1=False, p2=False),
    ]

output_sequence = []
for x in input_sequence:
    current_output = my_monitor.update(x)
    output_sequence.append(current_output)
    
print(output_sequence)
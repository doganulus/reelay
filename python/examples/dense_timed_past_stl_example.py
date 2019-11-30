import math
import reelay

my_monitor = reelay.past_stl.monitor(
    pattern="once[:0.1](speed > 0.7)", 
    piecewise="linear"
    )

timepoints = [0.03, 0.06, 0.08, 0.23, 0.33, 0.36, 0.38, 0.39]
input_sequence = [
    dict(time=t, speed=math.cos(2*math.pi*t)) for t in timepoints]

output_sequence = []
my_monitor.init_update(dict(time=0, speed=0.0))
for x in input_sequence:
    current_output = (dict(time=t, value=v) for t,v in my_monitor.update(x))
    output_sequence.extend(current_output)

print(input_sequence)
print(output_sequence)
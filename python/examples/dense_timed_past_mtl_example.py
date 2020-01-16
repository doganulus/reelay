import reelay

my_monitor = reelay.past_mtl.monitor(
    "p1 since[4.1:13.2] p2", 
    time_model="dense")

input_sequence = [
    dict(time=3.4,  p1=False,p2=False),
    dict(time=6.4,  p1=False,p2=True),
    dict(time=8.7,  p1=True, p2=False),
    dict(time=23.1, p1=True, p2=False),
    dict(time=33.2, p1=True, p2=False),
    dict(time=36.4, p1=True, p2=False),
    dict(time=38.7, p1=True, p2=False),
    dict(time=39.2, p1=True, p2=False),
    ]

output_sequence = []
for x in input_sequence:
    current_output = (dict(time=t, value=v) for t,v in my_monitor.update(x))
    output_sequence.extend(current_output)
    
print(output_sequence)
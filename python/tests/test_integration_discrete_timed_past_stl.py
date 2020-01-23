import reelay

def test_integration_discrete_timed_past_stl():

    my_monitor = reelay.past_stl.monitor(
        pattern="(speed > 13.0) since[:3] lights_on", 
        time_model="discrete"    
        )

    input_sequence = [
        dict(speed=3.3,  lights_on=False),
        dict(speed=6.3,  lights_on=False),
        dict(speed=9.3,  lights_on=True),
        dict(speed=13.3, lights_on=False),
        dict(speed=13.4, lights_on=False),
        dict(speed=13.3, lights_on=False),
        dict(speed=13.2, lights_on=False),
        dict(speed=13.2, lights_on=False),
        ]

    output_sequence = []
    for x in input_sequence:
        current_output = my_monitor.update(x)
        output_sequence.append(current_output)

    expected = [False, False, True, True, True, True, False, False]
    assert output_sequence == expected
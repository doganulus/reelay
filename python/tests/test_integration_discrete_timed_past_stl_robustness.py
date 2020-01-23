import pytest
import reelay

def test_integration_discrete_timed_past_stl_robustness():

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

    expected = [-9.7, -9.7, -9.7, -9.7, -9.7, -6.7, -3.7, 0.2, 0.2, 0.2, 0.2, 0.1, 0.1, 0.1, 0.1, 0.1, 0.4, 0.4]
    approx_expected = [pytest.approx(x, 0.001) for x in expected] 
    assert output_sequence == approx_expected
#
# Copyright (c) 2019-2020 Dogan Ulus
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
import pytest
import reelay


def test_integration_dense_timed_past_stl_robustness():

    my_monitor = reelay.past_stl.monitor(
        pattern=r"historically[:4.2]{speed < 13.0}",
        time_model="dense",
        piecewise="constant",
        semantics="robustness"
    )

    input_sequence = [
        dict(time=10.0, speed=3.3),
        dict(time=12.0, speed=9.3),
        dict(time=14.0, speed=13.3),
        dict(time=15.0, speed=11.3),
        dict(time=18.0, speed=9.4),
        dict(time=19.0, speed=6.3),
    ]

    output_sequence = []
    for x in input_sequence:
        current_output = (dict(time=t, value=v)
                          for t, v in my_monitor.update(x))
        output_sequence.extend(current_output)

    print(output_sequence)

    expected = [
        {'time': 10.0, 'value': 9.7},
        {'time': 12.0, 'value': 3.7},
        {'time': 14.0, 'value': -0.3},
        {'time': 15.0, 'value': -0.3},
        {'time': 18.0, 'value': -0.3},
        {'time': 18.2, 'value': -0.3},
        {'time': 19.0, 'value': 1.7}
    ]

    approx_expected = [pytest.approx(x, 0.001) for x in expected]

    assert output_sequence == approx_expected

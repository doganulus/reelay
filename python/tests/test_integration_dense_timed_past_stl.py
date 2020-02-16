#
# Copyright (c) 2019-2020 Dogan Ulus
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
import math
import pytest
import reelay


def test_integration_dense_timed_past_stl():

    my_monitor = reelay.past_stl.monitor(
        pattern=r"once[:0.1](speed > 0.7)",
        piecewise="linear"
    )

    timepoints = [0.03, 0.06, 0.08, 0.23, 0.33, 0.36, 0.38, 0.39]
    input_sequence = [
        dict(time=t, speed=math.cos(2*math.pi*t)) for t in timepoints]

    output_sequence = []
    my_monitor.init_update(dict(time=0, speed=0.0))
    for x in input_sequence:
        current_output = (dict(time=t, value=v)
                          for t, v in my_monitor.update(x))
        output_sequence.extend(current_output)

    expected = [
        {'time': pytest.approx(0.02137, 0.001), 'value': False},
        {'time': 0.03, 'value': True},
        {'time': 0.06, 'value': True},
        {'time': 0.08, 'value': True},
        {'time': pytest.approx(0.2152, 0.001), 'value': True},
        {'time': 0.23, 'value': False},
        {'time': 0.33, 'value': False},
        {'time': 0.36, 'value': False},
        {'time': 0.38, 'value': False},
        {'time': 0.39, 'value': False}
    ]

    assert output_sequence == expected

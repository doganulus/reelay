#
# Copyright (c) 2019-2020 Dogan Ulus
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
import pytest
import reelay


def test_integration_discrete_timed_past_mtl():

    my_monitor = reelay.past_mtl.monitor(
        pattern=r"{p1} since[4.1:13.2] {p2}",
        time_model="dense")

    input_sequence = [
        dict(time=3.4,  p1=False, p2=False),
        dict(time=6.4,  p1=False, p2=True),
        dict(time=8.7,  p1=True, p2=False),
        dict(time=23.1, p1=True, p2=False),
        dict(time=33.2, p1=True, p2=False),
        dict(time=36.4, p1=True, p2=False),
        dict(time=38.7, p1=True, p2=False),
        dict(time=39.2, p1=True, p2=False),
    ]

    output_sequence = []
    for x in input_sequence:
        current_output = (dict(time=t, value=v)
                          for t, v in my_monitor.update(x))
        output_sequence.extend(current_output)

    expected = [
        {'time': 3.4, 'value': False},
        {'time': 6.4, 'value': False},
        {'time': 8.7, 'value': False},
        {'time': pytest.approx(10.5, 0.001), 'value': False},
        {'time': pytest.approx(19.6, 0.001), 'value': True},
        {'time': 23.1, 'value': False},
        {'time': 33.2, 'value': False},
        {'time': 36.4, 'value': False},
        {'time': 38.7, 'value': False},
        {'time': 39.2, 'value': False}
    ]

    assert output_sequence == expected

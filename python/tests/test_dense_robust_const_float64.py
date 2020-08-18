#
# Copyright (c) 2019-2020 Dogan Ulus
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
import pytest

from reelay import dense_timed_monitor


def test_dense_robust_const_float64():

    my_monitor = dense_timed_monitor(
        pattern=r"{speed > 130} since[:40] {lights_on: true}",
        semantics="robustness",
        piecewise='constant'
    )

    input_sequence = [
        dict(time=0.0, speed=33,  lights_on=False),
        dict(time=12.0, speed=63,  lights_on=False),
        dict(time=14.0, speed=93,  lights_on=True),
        dict(time=18.0, speed=133, lights_on=False),
        dict(time=21.0, speed=134, lights_on=False),
        dict(time=26.0, speed=133, lights_on=False),
        dict(time=45.0, speed=132, lights_on=False),
        dict(time=68.0, speed=132, lights_on=False),
    ]

    result = []
    for x in input_sequence:
        chunk = my_monitor.update(x)
        result.extend(chunk)

    print(result)

    expected = [
        {'time': 0.0, 'value': -float('inf')},
        {'time': 14.0, 'value': float('inf')},
        {'time': 18.0, 'value': 3},
        {'time': 45.0, 'value': 2},
        {'time': 58.0, 'value': -float('inf')}
    ]

    approx_expected = [pytest.approx(x, 0.001) for x in expected]

    assert result == approx_expected

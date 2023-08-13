#
# Copyright (c) 2019-2020 Dogan Ulus
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
import pytest

from reelay import dense_timed_monitor


def test_dense_prop_const_float64():

    my_monitor = dense_timed_monitor(
        pattern="{speed > 12.0}",
        semantics="boolean",
        piecewise='constant'
    )

    input_sequence = [
        dict(time=0.0, speed=3.0),
        dict(time=10.0, speed=9.0),
        dict(time=12.0, speed=13.0),
        dict(time=14.0, speed=11.0),
        dict(time=15.0, speed=9.0),
        dict(time=18.0, speed=6.0),
        dict(time=19.0),
        ]

    result = []
    for x in input_sequence:
        chunk = my_monitor.update(x)
        print(chunk)
        result.extend(chunk)

    expected = [
        {'time': 0.0, 'value': False},
        {'time': 12.0, 'value': True},
        {'time': 14.0, 'value': False}
    ]

    approx_expected = [pytest.approx(x, 0.001) for x in expected]

    assert result == approx_expected

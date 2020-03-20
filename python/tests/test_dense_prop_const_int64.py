#
# Copyright (c) 2019-2020 Dogan Ulus
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
from reelay.monitors import dense_monitor


def test_dense_prop_const_int64():

    my_monitor = dense_monitor(
        pattern="{speed > 12.0}",
        semantics="boolean",
        piecewise='constant',
        timetype="float64")

    input_sequence = [
        dict(time=0, speed=3),
        dict(time=10, speed=9),
        dict(time=12, speed=13),
        dict(time=14, speed=11),
        dict(time=15, speed=9),
        dict(time=18, speed=6),
        dict(time=19),
        ]

    result = []
    for x in input_sequence:
        chunk = my_monitor.update(x)
        result.extend(chunk)

    expected = [
        {'time': 0, 'value': False},
        {'time': 12, 'value': True},
        {'time': 14, 'value': False}
    ]

    assert result == expected

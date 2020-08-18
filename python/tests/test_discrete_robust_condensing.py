#
# Copyright (c) 2019-2020 Dogan Ulus
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
import pytest

from reelay import discrete_timed_monitor


def test_discrete_robust():

    my_monitor = discrete_timed_monitor(
        pattern=r"{speed > 13.0} since[:3] {lights_on: true}",
        semantics="robustness",
        t_name="index",
        y_name="rval",
        condense=True
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

    result = []
    for x in input_sequence:
        y = my_monitor.update(x)
        result.append(y)

    expected = [
        {'index': 0, 'rval': -float('inf')},
        {},
        {'index': 2, 'rval': float('inf')},
        {'index': 3, 'rval': 0.3},
        {},
        {},
        {'index': 6, 'rval': -float('inf')},
        {}]

    approx_expected = [pytest.approx(x, 0.001) for x in expected]

    assert result == approx_expected

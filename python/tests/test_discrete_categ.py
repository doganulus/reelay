#
# Copyright (c) 2019-2020 Dogan Ulus
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
import pytest

from reelay import discrete_timed_monitor


def test_discrete_categ():

    my_monitor = discrete_timed_monitor(
        pattern=r"""forall[sensor].
            {sensor_id: *sensor, action: send_data}
                implies
            once[:4]{sensor_id: *sensor, action: calibrated}
            """,
        semantics="boolean",
        condense=False
    )

    input_sequence = [
        dict(sensor_id="1", action="calibrated"),
        dict(sensor_id="2", action="calibrated"),
        dict(sensor_id="1", action="send_data"),
        dict(sensor_id="1", action="send_data"),
        dict(sensor_id="1", action="send_data"),
        dict(sensor_id="1", action="calibrated"),
        dict(sensor_id="2", action="send_data"),
        dict(sensor_id="1", action="send_data"),
        dict(sensor_id="1", action="send_data"),
        dict(sensor_id="2", action="send_data"),
    ]

    result = []
    for x in input_sequence:
        y = my_monitor.update(x)
        result.append(y)

    expected = [
        {'value': True},
        {'value': True},
        {'value': True},
        {'value': True},
        {'value': True},
        {'value': True},
        {'value': False},
        {'value': True},
        {'value': True},
        {'value': False}]

    approx_expected = [pytest.approx(x, 0.001) for x in expected]

    assert result == approx_expected

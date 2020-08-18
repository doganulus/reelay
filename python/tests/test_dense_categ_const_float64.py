#
# Copyright (c) 2019-2020 Dogan Ulus
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
import pytest

from reelay import dense_timed_monitor


def test_dense_categ_const_float64():

    my_monitor = dense_timed_monitor(
        pattern=r"""forall[sensor].
            {sensor_id: *sensor, action: send_data}
                implies
            once[:40]{sensor_id: *sensor, action: calibrated}
            """,
        semantics="boolean",
        piecewise='constant'
    )

    input_sequence = [
        dict(time=0.0),
        dict(time=6.0, sensor_id="1", action="calibrated"),
        dict(time=12.0, sensor_id="2", action="calibrated"),
        dict(time=15.0, sensor_id="1", action="send_data"),
        dict(time=24.0, sensor_id="1", action="send_data"),
        dict(time=27.0, sensor_id="1", action="send_data"),
        dict(time=32.0, sensor_id="1", action="calibrated"),
        dict(time=46.0, sensor_id="2", action="send_data"),
        dict(time=49.0, sensor_id="1", action="send_data"),
        dict(time=56.0, sensor_id="1", action="send_data"),
        dict(time=62.0, sensor_id="2", action="send_data"),
        dict(time=67.0, sensor_id="1", action="send_data"),
    ]

    result = []
    for x in input_sequence:
        chunk = my_monitor.update(x)
        print(chunk)
        result.extend(chunk)

    expected = [
        {'time': 0.0, 'value': True},
        {'time': 62.0, 'value': False},
    ]

    approx_expected = [pytest.approx(x, 0.001) for x in expected]

    assert result == approx_expected

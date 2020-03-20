#
# Copyright (c) 2019-2020 Dogan Ulus
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
from reelay.monitors import dense_monitor


def test_dense_categ_const_int64():

    my_monitor = dense_monitor(
        pattern=r"""forall[sensor].
            {sensor_id: *sensor, action: send_data}
                implies
            once[:40]{sensor_id: *sensor, action: calibrated}
            """,
        semantics="boolean",
        piecewise='constant',
        timetype="int64"
    )

    input_sequence = [
        dict(time=0),
        dict(time=6, sensor_id="1", action="calibrated"),
        dict(time=12, sensor_id="2", action="calibrated"),
        dict(time=15, sensor_id="1", action="send_data"),
        dict(time=24, sensor_id="1", action="send_data"),
        dict(time=27, sensor_id="1", action="send_data"),
        dict(time=32, sensor_id="1", action="calibrated"),
        dict(time=46, sensor_id="2", action="send_data"),
        dict(time=49, sensor_id="1", action="send_data"),
        dict(time=56, sensor_id="1", action="send_data"),
        dict(time=62, sensor_id="2", action="send_data"),
        dict(time=67, sensor_id="1", action="send_data"),
    ]

    result = []
    for x in input_sequence:
        chunk = my_monitor.update(x)
        result.extend(chunk)

    expected = [
        {'time': 0, 'value': True},
        {'time': 62, 'value': False}
    ]

    assert result == expected

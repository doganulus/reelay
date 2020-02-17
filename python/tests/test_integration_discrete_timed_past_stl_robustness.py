#
# Copyright (c) 2019-2020 Dogan Ulus
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
import pytest
import reelay


def test_integration_discrete_timed_past_stl_robustness():

    my_monitor = reelay.past_stl.monitor(
        pattern=r"historically[:4]{speed > 13.0}",
        time_model="discrete",
        semantics="robustness"
    )

    input_sequence = [
        dict(speed=3.3),
        dict(speed=6.3),
        dict(speed=9.3),
        dict(speed=13.3),
        dict(speed=13.4),
        dict(speed=13.3),
        dict(speed=13.2),
        dict(speed=13.2),
        dict(speed=13.4),
        dict(speed=13.4),
        dict(speed=13.3),
        dict(speed=13.1),
        dict(speed=13.4),
        dict(speed=13.4),
        dict(speed=13.4),
        dict(speed=13.4),
        dict(speed=13.4),
        dict(speed=13.4),
    ]

    output_sequence = []
    for x in input_sequence:
        current_output = my_monitor.update(x)
        output_sequence.append(current_output)

    expected = [-9.7, -9.7, -9.7, -9.7, -9.7, -6.7, -3.7,
                0.2, 0.2, 0.2, 0.2, 0.1, 0.1, 0.1, 0.1, 0.1, 0.4, 0.4]
    approx_expected = [pytest.approx(x, 0.001) for x in expected]

    assert output_sequence == approx_expected

#
# Copyright (c) 2019-2020 Dogan Ulus
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
import reelay


def test_integration_discrete_timed_past_mtl():

    my_monitor = reelay.past_mtl.monitor(
        pattern=r"p1 since[2:4] p2",
        time_model="discrete")

    input_sequence = [
        dict(p1=False, p2=False),
        dict(p1=False, p2=True),
        dict(p1=True,  p2=False),
        dict(p1=True,  p2=False),
        dict(p1=True,  p2=False),
        dict(p1=True,  p2=False),
        dict(p1=True,  p2=False),
        dict(p1=True,  p2=False),
    ]

    output_sequence = []
    for x in input_sequence:
        current_output = my_monitor.update(x)
        output_sequence.append(current_output)

    expected = [False, False, False, True, True, True, False, False]

    assert output_sequence == expected

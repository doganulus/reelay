#
# Copyright (c) 2019-2020 Dogan Ulus
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
import reelay


def test_integration_past_ltl():

    my_monitor = reelay.past_ltl.monitor(
        pattern=r"{p1} since {p2}")

    input_sequence = [
        dict(p1=False, p2=False),
        dict(p1=False, p2=True),
        dict(p1=True,  p2=False),
        dict(p1=True,  p2=False),
        dict(p1=True,  p2=False),
        dict(p1=True,  p2=False),
        dict(p1=True,  p2=False),
        dict(p1=False, p2=False),
        ]

    output_sequence = []
    for x in input_sequence:
        current_output = my_monitor.update(x)
        output_sequence.append(current_output)

    expected = [False, True, True, True, True, True, True, False]

    assert output_sequence == expected

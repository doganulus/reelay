#
# Copyright (c) 2019-2020 Dogan Ulus
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
import reelay


def test_integration_dense_timed_past_qtl_dict():

    my_monitor = reelay.past_qtl.monitor(
        pattern=r"""forall[file].(
            {event: close, file: *file} implies (
                exists[mode].
                    once[1:1](!{event: close, file: *file}
                    since[:100] {event:open, file: *file, mode: *mode})
                )
            )
        """,
        time_model="dense",
        event_type="dict[str,str]")

    event_sequence = [
        dict(time="1", event="open",  file="my_doc_1.txt", mode="read"),
        dict(time="2",event="open",  file="my_doc_2.txt", mode="read"),
        dict(time="3",event="open",  file="my_doc_3.txt", mode="read"),
        dict(time="4",event="open",  file="my_doc_4.txt", mode="read"),
        dict(time="5",event="close", file="my_doc_4.txt"),
        dict(time="6",event="open",  file="my_doc_3.txt", mode="write"),
        dict(time="7",event="open",  file="my_doc_4.txt", mode="write"),
        dict(time="8",event="close", file="my_doc_4.txt"),
        dict(time="9",event="close", file="my_doc_3.txt"),
        dict(time="10",event="close", file="my_doc_2.txt"),
        dict(time="11",event="close", file="my_doc_1.txt"),
        dict(time="12",event="close", file="my_doc_4.txt"),
    ]
    output_sequence = []
    for event in event_sequence:
        current_output = my_monitor.update(event)
        output_sequence.extend(current_output)

    expected = [(1, True),(2, True),(3, True),(4, True),(5, True),(6, True),(7, True),(8, True),(9, True),(10, True),(11, True),(12, False)]

    assert output_sequence == expected

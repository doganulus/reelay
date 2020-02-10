#
# Copyright (c) 2019-2020 Dogan Ulus
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
import reelay


def test_integration_dense_timed_past_qtl_list():

    my_monitor = reelay.past_qtl.monitor(
        pattern=r"""forall[file].(
            [close, *file] implies (
                exists[mode].
                    once[1:1](![close, *file] since[:10] [open, *file, *mode])))""",
        time_model="dense",
        event_type="list[str]")

    event_sequence = [
        ["1", "open", "my_doc_1.txt", "read"],
        ["2", "open", "my_doc_2.txt", "read"],
        ["3", "open", "my_doc_3.txt", "read"],
        ["4", "open", "my_doc_4.txt", "read"],
        ["5", "close", "my_doc_4.txt"],
        ["6", "open", "my_doc_3.txt", "write"],
        ["7", "open", "my_doc_4.txt", "write"],
        ["8", "close", "my_doc_4.txt"],
        ["9", "close", "my_doc_3.txt"],
        ["10", "close", "my_doc_2.txt"],
        ["11", "close", "my_doc_1.txt"],
        ["12", "close", "my_doc_4.txt"],
    ]

    output_sequence = []
    for event in event_sequence:
        current_output = my_monitor.update(event)
        output_sequence.extend(current_output)

    expected = [(1, True),(2, True),(3, True),(4, True),(5, True),(6, True),(7, True),(8, True),(9, True),(10, True),(11, True),(12, False)]

    assert output_sequence == expected

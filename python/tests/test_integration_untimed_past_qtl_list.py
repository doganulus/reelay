import reelay


def test_integration_untimed_past_qtl_list():

    my_monitor = reelay.past_qtl.monitor(
        pattern=r"""forall[file].(
            [close, *file] implies (
                exists[mode].
                    pre(![close, *file] since [open, *file, *mode])))""",
        event_type="list[str]")

    event_sequence = [
        ["open", "my_doc_1.txt", "read"],
        ["open", "my_doc_2.txt", "read"],
        ["open", "my_doc_3.txt", "read"],
        ["open", "my_doc_4.txt", "read"],
        ["close", "my_doc_4.txt"],
        ["open", "my_doc_3.txt", "write"],
        ["open", "my_doc_4.txt", "write"],
        ["close", "my_doc_4.txt"],
        ["close", "my_doc_3.txt"],
        ["close", "my_doc_2.txt"],
        ["close", "my_doc_1.txt"],
        ["close", "my_doc_4.txt"],
    ]

    output_sequence = []
    for event in event_sequence:
        current_output = my_monitor.update(event)
        output_sequence.append(current_output)

    expected = [True, True, True, True, True,
                True, True, True, True, True, True, False]

    assert output_sequence == expected

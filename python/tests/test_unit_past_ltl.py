import reelay


def test_negation():

    my_monitor = reelay.past_ltl.monitor(
        pattern="!p1")

    input_sequence = [
        dict(p1=False),
        dict(p1=True),
    ]

    output_sequence = []
    for x in input_sequence:
        current_output = my_monitor.update(x)
        output_sequence.append(current_output)

    expected = [True, False]

    assert output_sequence == expected


def test_disjunction():

    my_monitor = reelay.past_ltl.monitor(
        pattern="p1 || p2")

    input_sequence = [
        dict(p1=False, p2=False),
        dict(p1=False, p2=True),
        dict(p1=True,  p2=False),
        dict(p1=True,  p2=True),
    ]

    output_sequence = []
    for x in input_sequence:
        current_output = my_monitor.update(x)
        output_sequence.append(current_output)

    expected = [False, True, True, True]

    assert output_sequence == expected


def test_conjunction():

    my_monitor = reelay.past_ltl.monitor(
        pattern="p1 && p2")

    input_sequence = [
        dict(p1=False, p2=False),
        dict(p1=False, p2=True),
        dict(p1=True,  p2=False),
        dict(p1=True,  p2=True),
    ]

    output_sequence = []
    for x in input_sequence:
        current_output = my_monitor.update(x)
        output_sequence.append(current_output)

    expected = [False, False, False, True]

    assert output_sequence == expected


def test_implication():

    my_monitor = reelay.past_ltl.monitor(
        pattern="p1 -> p2")

    input_sequence = [
        dict(p1=False, p2=False),
        dict(p1=False, p2=True),
        dict(p1=True,  p2=False),
        dict(p1=True,  p2=True),
    ]

    output_sequence = []
    for x in input_sequence:
        current_output = my_monitor.update(x)
        output_sequence.append(current_output)

    expected = [True, True, False, True]

    assert output_sequence == expected


def test_since():

    my_monitor = reelay.past_ltl.monitor(
        pattern="p1 since p2")

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

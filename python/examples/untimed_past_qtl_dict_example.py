import reelay

my_monitor = reelay.past_qtl.monitor(
    pattern=r"""forall[file].(
        {event: close, file: *file} implies (
            exists[mode]. 
                pre(!{event: close, file: *file} 
                since {event:open, file: *file, mode: *mode})
            )
        )
    """,
    event_type="dict[str,str]")

event_sequence = [
    dict(event="open",  file="my_doc_1.txt", mode="read"),
    dict(event="open",  file="my_doc_2.txt", mode="read"),
    dict(event="open",  file="my_doc_3.txt", mode="read"),
    dict(event="open",  file="my_doc_4.txt", mode="read"),
    dict(event="close", file="my_doc_4.txt"),
    dict(event="open",  file="my_doc_3.txt", mode="write"),
    dict(event="open",  file="my_doc_4.txt", mode="write"),
    dict(event="close", file="my_doc_4.txt"),
    dict(event="close", file="my_doc_3.txt"),
    dict(event="close", file="my_doc_2.txt"),
    dict(event="close", file="my_doc_1.txt"),
    dict(event="close", file="my_doc_4.txt"),
]

output_sequence = []
for event in event_sequence:
    current_output = my_monitor.update(event)
    output_sequence.append(current_output)
    
print(output_sequence)
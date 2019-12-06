# Getting Started From Python

!!! Warning
    Reelay Python API is in its very early stage. Everthing in this page is subject to change.

| Requirement  | Specification                                                                 | Failure Type   |
|--------------|-------------------------------------------------------------------------------|----------------|
| `SYS-REQ-01` | `(historically[0:15](door_open) and not dow_suppressed) -> door_open_warning` | False Negative |
| `SYS-REQ-01` | `door_open_warning -> historically[0:15](door_open)`                          | False Positive |
| `SYS-REQ-01` | `door_open_warning -> not dow_suppressed`                                     | False Positive |
| `SYS-REQ-02` | `door_open_warning -> not(pre(door_open since door_open_warning))`            | False Positive |

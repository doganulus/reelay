# Tutorial: Introduction

This initial part of the tutorial is about why we have system requirements and how to write them in the specification languages that Reelay support. The remaining parts will about how to check these specifications over temporal streams using **Reelay**.

Let’s start with an example from the robotics such that we are designing a home assistant robot as our next big product. Besides we also think our customers would love our robots with a door open warning feature! 

## An Example Feature

So we want to design a feature that warns the user whenever our robot detects the entry door is left open. For the Door Open Warning (DOW) feature, there are two classes of failures, false positives (FP) and false negatives (FN). If we were more functional safety minded, there will be the assessment of the severity of each class but let's leave such aspects for another tutorial. We will simply measure the progress of development by the number of failures over some set of test cases.

The next step is to analyse and break down the feature into testable system requirements. Perhaps we would want to issue a warning after some time, say 15 minutes, rather than immediately. This constitutes our first requirement expressed as follows:

!!! Requirement
    **SYS-REQ-01:** The system shall issue a warning if the entry door is left open more than 15 minutes.

An important point here is that we can actually check this requirement over the behavior of the robot in such scenarios. Perhaps a spoiler but we will use **Reelay** monitors to check but let's finish our requirements first. Assume that we also want to give the warning once as multiple warnings for the same occasion may annoy our users. This constitutes our third and final requirement expressed as follows:

!!! Requirement
    **SYS-REQ-02:** The door should be closed before the system issues another warning. This should prevent that the warning is issuing several warnings after another, while the door is open.

Now we have a set of system requirements for the DOW feature. The next section will be about writing these requirements in a non-ambigious (and executable) specification language.

## Specify Requirements

In this tutorial, we will use Past Temporal Logic (PTL) to specify our requirements. Temporal logics such as PTL are commonly used to describe temporal patterns over signals in a precise manner. The cheatsheet for PTL is given [here](past_temporal_logic.md). More complete information and historical background of temporal logics can be found [here](temporal_logic.md). 

Let's specify!

Our first requirement `SYS-REQ-01` says that the system shall issue a warning if the door is open more than 15 minutes. Our common sense says that this should happen if the warning is not suppressed. Therefore we have to take care of such considerations when translating English into formal specifications.

```
(historically[0:15](door_open) and not dow_suppressed) -> door_open_warning
```
This specification tests `SYS-REQ-01` and its violation would be considered as a false negative.

In plain English, the conditional connective `if` has sometimes a bi-directional meaning where the reverse case goes without saying. 
The remaining two specifications test for two types of false positives, respectively. It is usually a good practice to break down specifications into smaller pieces as it would be easier to debug if tests fail.
```
door_open_warning -> historically[0:15](door_open)
```
```
door_open_warning -> not dow_suppressed
```
These specifications test `SYS-REQ-01` and each of their violations would be considered as a false positive.

The seconf requirement `SYS-REQ-02`
```
door_open_warning -> not(pre(door_open since door_open_warning))
```
These specifications test `SYS-REQ-01` and each of their violations would be considered as a false positive.


Finally let's tabulate our formal specifications obtained from system requirements in a table nicely as follows:

| Requirement  | Specification                                                                 | Failure Type   |
|--------------|-------------------------------------------------------------------------------|----------------|
| `SYS-REQ-01` | `(historically[0:15](door_open) and not dow_suppressed) -> door_open_warning` | False Negative |
| `SYS-REQ-01` | `door_open_warning -> historically[0:15](door_open)`                          | False Positive |
| `SYS-REQ-01` | `door_open_warning -> not dow_suppressed`                                     | False Positive |
| `SYS-REQ-02` | `door_open_warning -> not(pre(door_open since door_open_warning))`            | False Positive |


## Verify the Execution

Following tutorials will explain how to check these specifications over the system behavior:

  * [Using Reelay monitors from C++](gs_cpp.md)
  * [Using Reelay monitors from Python](gs_python.md)
  * [Using Reelay monitors from CLI](gs_cli.md)

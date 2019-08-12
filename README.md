# Reelay

# Install

# Getting Started

# Current Roadmap

v.1.1 -- Untimed, discrete and dense timed settings for past temporal logic (Past LTL/MTL)
v.1.2 -- Release MTL application

v.1.3 -- Predicate support over piecewise linear signals (Past STL)
v.1.4 -- Robustness semantics (Past rSTL)
v.1.5 -- Release STL application

v.1.6 -- Release Python bindings

v.1.x -- Untimed regular expressions (RE)
v.1.x -- Timed regular expressions (TRE)
v.1.x -- Release REGEX application

v.1.x -- First-order quantification for untimed past temporal logic (Past FOLTL)
v.1.x -- First-order quantification for timed past temporal logic (Past FOMTL)
v.1.x -- Release FOL application

v.2.0 -- Release Reelay 2.0!

# FAQ

## Why past temporal logic? Most work has been done on the future fragment. 

The shortest answer is because of past time paradise.

## Ok but still I need to use the future fragment. Can I do that using reelay or there will be a support in the future (no pun intended)?

First (un)solution: Do not use the future fragment for online monitoring. Most commonly used properties have similar formula lengths when expressed in past and future fragments. The legacy of the future fragment comes from model checking, which has very different rationale than runtime verification.

The concern about the causality is for online monitoring applications. 

Currently Reelay can be used for offline monitoring of the future fragment as follows:
    1. Translate the future formula into a dual past formula.
    2. Reverse the input sequence (Clearly this is impossible for online monitoring applications).
    3. Execute the monitor for the past formula over the reversed sequence.

This would do the trick due to the symmetric definition of past and future fragments.

In the future, there will be no support for online monitoring of the future fragment. 

## I have studied mathematics. Your dense time formulation seems weird. Dense time sequences are not compressed discrete time sequences.

Actually they are! Ok, ok, isomorphic! Both in practice and in constructive mathematics. However, I'd say that non-constructive maths is the one that is weird. :)
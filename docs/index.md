# Reelay Monitors

Reelay is a header-only C++ library for system-level verification and testing of real-time systems. Reelay implements state-of-the-art [runtime verification](https://en.wikipedia.org/wiki/Runtime_verification) techniques to construct runtime monitors that check temporal behaviors of the system against system-level requirements. Hence, Reelay can be used to enhance rigorous systems engineering practices by formalizing and automating the assessment phase. 

## Features
* Formal specification of temporal properties
* Provably correct monitor construction from the specification
* Fast and frugal runtime requirement checking (very low overhead)
* Simple but non-restrictive user interface 
* Available for C++, Python, and the command line

## Rye Format

Reelay reads executable specifications written in plain text and verifies at runtime that the system does what those specifications describe. In order to construct runtime monitors automatically, those specifications must follow some basic syntax rules, called Reelay Expression (Rye) format. See [Rye format reference](rye.md) for more details.

## Get Started

The next steps for you are to install Reelay from [source code](install.md), [pip](install.md), or [Docker image](docker.md) and follow through [the tutorial](gs_intro) to learn how to use Reelay monitors from [C++](gs_cpp.md) and [Python](gs_python).

## Contribute

Currently Reelay does not have a contribution guideline. However, we always welcome bug reports, enhancements, and comments regarding the project. Please use the [Issues](https://github.com/doganulus/reelay/issues) page if you have a bug report, enhancement, or comment.

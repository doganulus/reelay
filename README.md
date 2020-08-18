# Reelay Monitors

![library](https://github.com/doganulus/reelay/workflows/library/badge.svg)![python](https://github.com/doganulus/reelay/workflows/python/badge.svg)![docs](https://github.com/doganulus/reelay/workflows/docs/badge.svg)[![Coverage](https://coveralls.io/repos/github/doganulus/reelay/badge.svg?branch=master)](https://coveralls.io/github/doganulus/reelay?branch=master)[![Codacy Badge](https://api.codacy.com/project/badge/Grade/9493a2a1ed2b47e6a0cfdcf6a68cd9b8)](https://www.codacy.com/manual/doganulus/reelay?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=doganulus/reelay&amp;utm_campaign=Badge_Grade)

Reelay is a header-only C++ library and set of tools for system-level verification and testing of real-time systems. Reelay implements state-of-the-art [runtime verification](https://en.wikipedia.org/wiki/Runtime_verification) techniques to construct runtime monitors that check temporal behaviors of the system against system-level requirements. Hence, Reelay can be used to enhance rigorous systems engineering practices by formalizing and automating the assessment phase.

## Main Features

* Formal specification of temporal properties
* Provably correct monitor construction from the specification
* Fast and frugal runtime requirement checking (very low overhead)
* Simple but non-restrictive user interface
* Available for C++ and Python

## Rye Format

Reelay reads executable specifications written in plain text and verifies that the system does what those specifications say at runtime. In order to construct runtime monitors automatically, those specifications must follow some basic syntax rules, called Reelay Expression (Rye) format. See the reference for the [Rye format](docs/rye.md) for more details.

## Get Started

The next steps for you are to install Reelay [from source](docs/install.md) and follow through [the tutorial](docs/gs_intro.md) to learn how to use Reelay monitors from [C++](docs/gs_cpp.md) and [Python](docs/gs_python.md). Check out [the documentation](https://doganulus.github.io/reelay/) for more details.

## Contribute

Currently Reelay does not have a contribution guideline. However, we always welcome bug reports, enhancements, and comments regarding the project. Please use the [Issues](https://github.com/doganulus/reelay/issues) page if you have a bug report, enhancement, or comment.

## Cite

If you are using Reelay in an academic work, please cite 

* [Online monitoring of metric temporal logic using sequential networks](https://arxiv.org/abs/1901.00175)

where the main technique has been explained throughtly. Further bits of techniques and motivations can be traced in the following papers.

* [First-order temporal logic monitoring with BDDs](https://link.springer.com/article/10.1007/s10703-018-00327-4)
* [Sequential circuits from regular expressions revisited](https://arxiv.org/abs/1801.08979)

Please also look at the [performance notes](docs/performance.md) if you compare the performance between tools.
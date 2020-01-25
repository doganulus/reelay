# Reelay Monitors

![library](https://github.com/doganulus/reelay/workflows/library/badge.svg)![apps](https://github.com/doganulus/reelay/workflows/apps/badge.svg)![python](https://github.com/doganulus/reelay/workflows/python/badge.svg)![docs](https://github.com/doganulus/reelay/workflows/docs/badge.svg)[![Codacy Badge](https://api.codacy.com/project/badge/Grade/9493a2a1ed2b47e6a0cfdcf6a68cd9b8)](https://www.codacy.com/manual/doganulus/reelay?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=doganulus/reelay&amp;utm_campaign=Badge_Grade)

Reelay is a header-only C++ library and set of tools for system-level verification and testing of real-time systems. Reelay implements state-of-the-art [runtime verification](https://en.wikipedia.org/wiki/Runtime_verification) techniques to construct runtime monitors that check temporal behaviors of the system against system-level requirements. Hence, Reelay can be used to enhance rigorous systems engineering practices by formalizing and automating the assessment phase.

## Main Features

* Formal specification of temporal properties
* Provably correct monitor construction from the specification
* Fast and frugal runtime requirement checking (very low overhead)
* Simple but non-restrictive user interface
* Available for C++, Python, and the command line

## Get Started

The next steps for you are to install Reelay [from source](docs/install.md) and follow through [the tutorial](docs/gs_intro.md) to learn how to use Reelay monitors from [C++](docs/gs_cpp.md), [Python](docs/gs_python.md), and the [command line interface](docs/gs_cli.md). Check out [the documentation](https://doganulus.github.io/reelay/) for more details.

## Contribute

Currently Reelay does not have a contribution guideline. However, we always welcome bug reports, enhancements, and comments regarding the project. Please use the [Issues](https://github.com/doganulus/reelay/issues) page if you have a bug report, enhancement, or comment. Thanks for your interest!

## Roadmap

Reelay is an ongoing project and below is a number of features and plans to expand the scope and improve the usability of the library.

| | Version | Feature                                                            |
|-|---------|--------------------------------------------------------------------|
|✅| v.1.1   | Untimed, discrete and dense timed settings for past temporal logic |
|✅| v.1.2   | MTL monitoring application (rymtl)                                 |
|✅| v.1.3   | Full predicate support over piecewise linear signals               |
|✅| v.1.4   | Robustness settings                                                |
|✅| v.1.5   | STL monitoring application (rystl)                                 |
|✅| v.1.6   | Python bindings and API                                            |
|✅| v.1.7   | First-order quantification for untimed past temporal logic         |
|🤞| v.1.x   | First-order quantification for past metric temporal logic          |
|🤞| v.1.x   | FOL monitoring application (ryjavu)                                |
|🤞| v.1.x   | Untimed regular expressions                                        |
|🤞| v.1.x   | Discrete and dense timed regular expressions                       |
|🤞| v.1.x   | TRE monitoring application                                         |

## Credits

This project has benefited from several other great software at various stages now and in the past.

1. [Catch2](https://github.com/catchorg/Catch2) for unit testing.
2. [pybind11](https://github.com/pybind/pybind11) for Python bindings.
3. Yuji Hirose's [cpp-peglib](https://github.com/yhirose/cpp-peglib) for specification parsing from C++.
4. Vincent La's [csv-parser](https://github.com/vincentlaucsb/csv-parser), Andrew DeOrio's [csvstream](https://github.com/awdeorio/csvstream), Ben Strasser's [fast-cpp-csv-parser](https://github.com/ben-strasser/fast-cpp-csv-parser) for CSV parsing from C++.
5. Pranav Srinivas Kumar's [argparse](https://github.com/p-ranav/argparse) for argument parsing from C++.
6. [mkdocs](https://github.com/mkdocs/mkdocs) and [mkdocs-material](https://github.com/squidfunk/mkdocs-material) for documentation website generation.

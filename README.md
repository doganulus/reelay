# Reelay Monitors
![](https://github.com/doganulus/reelay/workflows/library/badge.svg)![](https://github.com/doganulus/reelay/workflows/apps/badge.svg)![](https://github.com/doganulus/reelay/workflows/python/badge.svg)![](https://github.com/doganulus/reelay/workflows/docs/badge.svg) [![Codacy Badge](https://api.codacy.com/project/badge/Grade/9493a2a1ed2b47e6a0cfdcf6a68cd9b8)](https://www.codacy.com/manual/doganulus/reelay?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=doganulus/reelay&amp;utm_campaign=Badge_Grade)

Reelay is a header-only C++ library and set of tools for system-level verification and testing of real-time systems. Reelay implements state-of-the-art [runtime verification](https://en.wikipedia.org/wiki/Runtime_verification) techniques to construct runtime monitors that check temporal behaviors of the system against system-level requirements. Hence, Reelay can be used to enhance rigorous systems engineering practices by formalizing and automating the assessment phase. See [the Reelay documentation](https://doganulus.github.io/reelay/) for details.

## Main Features
  * Formal specification of temporal properties
  * Provably correct monitor construction from the specification
  * Fast and frugal runtime requirement checking (very low overhead)
  * Simple but non-restrictive user interface 
  * Available for C++, Python, and the command line

## Get Started

The next steps for you are to install Reelay [from source](docs/install.md) or [Docker image](docs/docker.md) and follow through tutorials to learn how to use Reelay monitors from [C++](docs/gs_cpp.md), [Python](docs/gs_python) and the [command line interface](docs/gs_cli.md).

## Contribute

Currently Reelay does not have a contribution guideline. However, we always welcome bug reports, enhancements, and comments regarding the project. Please use the [Issues](https://github.com/doganulus/reelay/issues) page if you have a bug report, enhancement, or comment. Thanks for your interest!

## Roadmap

Reelay is an ongoing project and below is a number of features and plans to expand the scope and improve the usability of the library.

| Status | Version | Deliverable | Codename |
|-|---------|-------------|----------|
|✅| v.1.1 | Untimed, discrete and dense timed settings for past temporal logic | MTL |
|✅| v.1.2 | MTL monitoring application | rymtl |
|✅| v.1.3 | Full predicate support over piecewise linear signals | STL |
|✅| v.1.4 | Robustness settings | STLRO |
|✅| v.1.5 | STL monitoring application | rystl
|✅| v.1.6 | Python bindings | MONPY |
|🤞| v.1.x | Untimed regular expressions | RE |
|🤞| v.1.x | Discrete and dense timed regular expressions | TRE |
|🤞| v.1.x | TRE monitoring application | rytre |
|🤞| v.1.x | First-order quantification for untimed past temporal logic | FOLTL |
|🤞| v.1.x | First-order quantification for past metric temporal logic | FOMTL |
|🤞| v.1.x | FOL monitoring application | ryjavu |
|🤞| v.2.0 | Reelay 2.0! | REELAY |
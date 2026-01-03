#!/usr/bin/env python3
"""
Copyright (c) 2019-2025 Dogan Ulus

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
"""

import sys
import argparse
import json
import reelay


def discrete_timed_processing(monitor, filename):
    """Process discrete-timed monitoring."""
    stdout_line_count = 0
    output_filename = filename + ".ryl"

    print(f"Processing {filename}")
    print("---")

    with open(filename, 'r') as infile, open(output_filename, 'w') as outfile:
        for line in infile:
            if line.strip():
                doc = json.loads(line)
                result = monitor.update(doc)

                if result:
                    if stdout_line_count < 5:
                        print(json.dumps(result))
                        stdout_line_count += 1
                    elif stdout_line_count == 5:
                        print("...")
                        stdout_line_count += 1
                    outfile.write(json.dumps(result) + "\n")

    if stdout_line_count < 5:
        print("---")
    print(f"Full output written to {output_filename}")


def dense_timed_processing(monitor, filename):
    """Process dense-timed monitoring."""
    stdout_line_count = 0
    output_filename = filename + ".ryl"

    print(f"Processing {filename}")
    print("---")

    with open(filename, 'r') as infile, open(output_filename, 'w') as outfile:
        for line in infile:
            if line.strip():
                doc = json.loads(line)
                result = monitor.update(doc)

                if result:
                    # Handle result as list of items for dense time
                    items = result if isinstance(result, list) else [result]
                    for item in items:
                        if stdout_line_count < 5:
                            print(json.dumps(item))
                            stdout_line_count += 1
                        elif stdout_line_count == 5:
                            print("...")
                            stdout_line_count += 1
                        outfile.write(json.dumps(item) + "\n")

    if stdout_line_count < 5:
        print("---")
    print(f"Full output written to {output_filename}")


def main(argv=None):
    if argv is None:
        argv = sys.argv

    parser = argparse.ArgumentParser(
        prog='reelay',
        description='Reelay Command Line Interface',
        epilog='Further information: https://github.com/doganulus/reelay'
    )

    # Positional arguments
    parser.add_argument('spec', help='Specification string')
    parser.add_argument('files', nargs='+', help='Input JSON files')

    # Time model options
    time_group = parser.add_mutually_exclusive_group()
    time_group.add_argument('-v', '--dense', action='store_true',
                           help='Use dense time model (default)')
    time_group.add_argument('-x', '--discrete', action='store_true',
                           help='Use discrete time model')

    # Time type options
    type_group = parser.add_mutually_exclusive_group()
    type_group.add_argument('-i', '--itime', action='store_true',
                           help='Use int64 as time type (default)')
    type_group.add_argument('-f', '--ftime', action='store_true',
                           help='with -v, use float64 as time type')

    # Semantics options
    semantics_group = parser.add_mutually_exclusive_group()
    semantics_group.add_argument('-b', '--boolean', action='store_true',
                                help='Use boolean semantics')
    semantics_group.add_argument('-r', '--robustness', action='store_true',
                                help='Use robustness semantics')

    # Interpolation options
    interp_group = parser.add_mutually_exclusive_group()
    interp_group.add_argument('-k', '--pwc', action='store_true',
                             help='with -v, use piecewise constant interpolation (default)')
    interp_group.add_argument('-l', '--pwl', action='store_true',
                             help='with -vf, use piecewise linear interpolation')

    # Other options
    parser.add_argument('-z', '--no-condense', action='store_true',
                       help='with -x, disable dense output')
    parser.add_argument('--tname', default='time', metavar='STRING',
                       help='Use STRING as the name of time field (default: time)')
    parser.add_argument('--yname', default='value', metavar='STRING',
                       help='Use STRING as the name of output field (default: value)')

    parser.add_argument('--version', action='version', version='ryjson 1.0')

    args = parser.parse_args(argv[1:])

    # Apply defaults
    use_discrete = args.discrete if args.discrete else not args.dense
    use_dense = args.dense if args.dense else not args.discrete
    use_boolean = args.boolean if args.boolean else not args.robustness
    use_robustness = args.robustness
    use_integer = args.itime if args.itime else not args.ftime
    use_floating = args.ftime
    use_constant = args.pwc if args.pwc else not args.pwl
    use_linear = args.pwl

    # Create monitor with appropriate options
    monitor = None

    if use_discrete and use_boolean:  # -x -xb -xbz
        monitor = reelay.discrete_timed_monitor(
            pattern=args.spec,
            semantics='boolean',
            t_name=args.tname,
            y_name=args.yname,
            condense=not args.no_condense
        )
    elif use_discrete and use_robustness:  # -xr -xrz
        monitor = reelay.discrete_timed_monitor(
            pattern=args.spec,
            semantics='robustness',
            t_name=args.tname,
            y_name=args.yname,
            condense=not args.no_condense
        )
    elif use_dense and use_boolean and use_integer:  # -vbi
        monitor = reelay.dense_timed_monitor(
            pattern=args.spec,
            semantics='boolean',
            t_name=args.tname,
            y_name=args.yname,
        )
    elif use_dense and use_boolean and use_floating and use_constant:  # -vbf, -vbfk
        monitor = reelay.dense_timed_monitor(
            pattern=args.spec,
            semantics='boolean',
            t_name=args.tname,
            y_name=args.yname,
            interpolation='constant'
        )
    elif use_dense and use_boolean and use_floating and use_linear:  # -vbfl
        monitor = reelay.dense_timed_monitor(
            pattern=args.spec,
            semantics='boolean',
            t_name=args.tname,
            y_name=args.yname,
            interpolation='linear'
        )
    elif use_dense and use_robustness and use_integer:  # -vri
        monitor = reelay.dense_timed_monitor(
            pattern=args.spec,
            semantics='robustness',
            t_name=args.tname,
            y_name=args.yname,
        )
    elif use_dense and use_robustness and use_floating:  # -vrf
        monitor = reelay.dense_timed_monitor(
            pattern=args.spec,
            semantics='robustness',
            t_name=args.tname,
            y_name=args.yname,
        )
    else:
        raise ValueError("Unsupported flag combination")

    # Process files
    if use_discrete:
        for filename in args.files:
            discrete_timed_processing(monitor, filename)
    else:
        for filename in args.files:
            dense_timed_processing(monitor, filename)


if __name__ == '__main__':
    main()

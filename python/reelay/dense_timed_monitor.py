#!/usr/bin/env python3
#
# Copyright (c) 2019-2023 Dogan Ulus
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
from reelay import _pybind11_module as rym


class dense_timed_monitor(object):
    def __new__(cls,
                pattern: str,
                semantics="boolean",   # boolean, robustness
                piecewise="constant",  # constant, linear
                t_name="time",         # any unique identifier
                y_name="value",        # any unique identifier
                ):

        inspection = rym.inspect(pattern)
        has_references = inspection['has_references']

        options = rym.monitor_options(
            t_name, y_name, piecewise, True)

        if not has_references and semantics == "boolean":
            if piecewise == "constant" or piecewise == "linear":
                return rym.dense_monitor.make(pattern, options)
            else:
                raise AttributeError(
                    "piecewise must be either 'constant' or 'linear'.")
        elif not has_references and semantics == "robustness":
            if piecewise == "constant":
                return rym.dense_robustness_monitor.make(
                    pattern, options)
            elif piecewise == "linear":
                raise AttributeError((
                    "Robustness semantics is not available for piecewise "
                    "linear interpolation. Consider piecewise constant "
                    "interpolation."))
            else:
                raise AttributeError(
                    "piecewise must be either 'constant' or 'linear'.")
        elif has_references and semantics == "boolean":
            if piecewise == "constant":
                return rym.dense_data_monitor.make(pattern, options)
            elif piecewise == "linear":
                raise AttributeError((
                    "Piecewise linear interpolation is not available for "
                    "specifications that contain data references. "
                    "Consider piecewise constant interpolation."))
            else:
                raise AttributeError(
                    "piecewise must be 'constant' or 'linear'.")
        elif has_references and semantics == "robustness":
            raise AttributeError((
                "Robustness semantics is not available for specifications"
                " that contain data references. Consider boolean semantics."))
        else:
            raise AttributeError(
                "semantics must be either 'boolean' or 'robustness'.")

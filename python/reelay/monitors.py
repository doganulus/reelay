#!/usr/bin/env python3
#
# Copyright (c) 2019-2020 Dogan Ulus
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
import reelay.library


class discrete_monitor(object):
    def __new__(cls,
                pattern: str,
                semantics="boolean",  # boolean, robustness
                t_name="time",        # any unique identifier
                y_name="value",       # any unique identifier
                condense=True,
                ):

        inspection = reelay.library.inspect(pattern)
        has_references = inspection['has_references']

        if not has_references and semantics == "boolean":
            if condense:
                return reelay.library.monitor_discrete_prop_condensing(
                    pattern, t_name, y_name)
            else:
                return reelay.library.monitor_discrete_prop(
                    pattern, t_name, y_name)
        elif not has_references and semantics == "robustness":
            if condense:
                return reelay.library.monitor_discrete_robust_condensing(
                    pattern, t_name, y_name)
            else:
                return reelay.library.monitor_discrete_robust(
                    pattern, t_name, y_name)
        elif has_references and semantics == "boolean":
            if condense:
                return reelay.library.monitor_discrete_categ_condensing(
                    pattern, t_name, y_name)
            else:
                return reelay.library.monitor_discrete_categ(
                    pattern, t_name, y_name)
        elif has_references and semantics == "robustness":
            raise AttributeError((
                "Robustness semantics is not available for specifications "
                "that contain data references. You may consider boolean "
                "semantics."))
        else:
            raise AttributeError(
                "semantics must be either 'boolean' or 'robustness'.")


class dense_monitor(object):
    def __new__(cls,
                pattern: str,
                semantics="boolean",   # boolean, robustness
                piecewise="constant",  # constant, linear
                timetype="int64",      # int64, float64
                t_name="time",         # any unique identifier
                y_name="value",        # any unique identifier
                ):

        inspection = reelay.library.inspect(pattern)
        has_references = inspection['has_references']

        if not has_references and semantics == "boolean":
            if piecewise == "constant":
                if timetype == "int64":
                    return reelay.library.monitor_dense_prop_const_int64(
                        pattern, t_name, y_name)
                elif timetype == "float64":
                    return reelay.library.monitor_dense_prop_const_float64(
                        pattern, t_name, y_name)
                else:
                    raise AttributeError(
                        "timetype must be either 'int64' or 'float64'.")
            elif piecewise == "linear":
                if timetype == "int64":
                    raise AttributeError((
                        "Linear interpolation over integers does not work. "
                        "Consider floating point numbers."))
                elif timetype == "float64":
                    return reelay.library.monitor_dense_prop_linear_float64(
                        pattern, t_name, y_name)
                else:
                    raise AttributeError(
                        "timetype must be either 'int64' or 'float64'.")
            else:
                raise AttributeError(
                    "piecewise must be either 'constant' or 'linear'.")
        elif not has_references and semantics == "robustness":
            if piecewise == "constant":
                if timetype == "int64":
                    return reelay.library.monitor_dense_robust_const_int64(
                        pattern, t_name, y_name)
                elif timetype == "float64":
                    return reelay.library.monitor_dense_robust_const_float64(
                        pattern, t_name, y_name)
                else:
                    raise AttributeError(
                        "timetype must be either 'int64' or 'float64'.")
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
                if timetype == "int64":
                    return reelay.library.monitor_dense_categ_const_int64(
                        pattern, t_name, y_name)
                elif timetype == "float64":
                    return reelay.library.monitor_dense_categ_const_float64(
                        pattern, t_name, y_name)
                else:
                    raise AttributeError(
                        "timetype must be either 'int64' or 'float64'.")
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

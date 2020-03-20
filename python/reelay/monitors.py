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
                y_name="value",        # any unique identifier
                ):

        inspection = reelay.library.inspect(pattern)
        has_references = inspection['has_references']

        if not has_references and semantics == "boolean":
            return reelay.library.monitor_discrete_prop(
                pattern, y_name)
        elif not has_references and semantics == "robustness":
            return reelay.library.monitor_discrete_robust(
                pattern, y_name)
        elif has_references and semantics == "boolean":
            return reelay.library.monitor_discrete_categ(
                pattern, y_name)
        elif has_references and semantics == "robustness":
            raise AttributeError(
                """Robustness semantics is not available for specifications that contain data references. You may consider semantics='boolean'.""")
        else:
            raise AttributeError(
                "semantics must be either 'boolean' or 'robustness'.")


class dense_monitor(object):
    def __new__(cls,
                pattern: str,
                semantics="boolean",   # boolean, robustness
                piecewise="constant",  # constant, linear
                timetype="float64",    # int64, float64
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
                    AttributeError(
                        "timetype must be either 'int64' or 'float64'.")
            elif piecewise == "linear":
                if timetype == "int64":
                    AttributeError(
                        "Linear interpolation over integers is not well behaving. You may consider timetype='float64'.")
                elif timetype == "float64":
                    return reelay.library.monitor_dense_prop_linear_float64(
                        pattern, t_name, y_name)
                else:
                    AttributeError(
                        "timetype must be either 'int64' or 'float64'.")
            else:
                AttributeError(
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
                    AttributeError(
                        "timetype must be either 'int64' or 'float64'.")
            elif piecewise == "linear":
                AttributeError(
                    """Robustness semantics is not available for piecewise linear interpolation. You may consider piecewise='constant'.""")
            else:
                AttributeError(
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
                    AttributeError(
                        "timetype must be either 'int64' or 'float64'.")
            elif piecewise == "linear":
                AttributeError(
                    """Piecewise linear interpolation is not available for specifications that contain data references. You may consider piecewise='constant'.""")
            else:
                AttributeError(
                    "piecewise must be 'constant' or 'linear'.")
        elif has_references and semantics == "robustness":
            raise AttributeError(
                """Robustness semantics is not available for specifications that contain data references. You may consider semantics='boolean'.""")
        else:
            raise AttributeError(
                "semantics must be either 'boolean' or 'robustness'.")

#!/usr/bin/env python3
#
# Copyright (c) 2019-2023 Dogan Ulus
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
from reelay import _pybind11_module as module


class discrete_timed_monitor(object):
    def __new__(cls,
                pattern: str,
                semantics="boolean",  # boolean, robustness
                t_name="time",        # any unique identifier
                y_name="value",       # any unique identifier
                condense=True,
                ):

        inspection = reelay.module.inspect(pattern)
        has_references = inspection['has_references']

        options = reelay.module.monitor_options(
            t_name, y_name, condense, True)

        if not has_references and semantics == "boolean":
            if condense:
                return reelay.module.condensing_monitor.make(
                    pattern, options)
            else:
                return reelay.module.discrete_monitor.make(
                    pattern, options)
        elif not has_references and semantics == "robustness":
            if condense:
                return reelay.module.condensing_robustness_monitor.make(
                    pattern, options)
            else:
                return reelay.module.discrete_robustness_monitor.make(
                    pattern, options)
        elif has_references and semantics == "boolean":
            if condense:
                return reelay.module.condensing_data_monitor.make(
                    pattern, options)
            else:
                return reelay.module.discrete_data_monitor.make(
                    pattern, options)
        elif has_references and semantics == "robustness":
            raise AttributeError((
                "Robustness semantics is not available for specifications "
                "that contain data references. You may consider boolean "
                "semantics."))
        else:
            raise AttributeError(
                "semantics must be either 'boolean' or 'robustness'.")

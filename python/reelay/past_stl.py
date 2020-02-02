#
# Copyright (c) 2019-2020 Dogan Ulus
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
import reelay.recipes


class monitor(object):
    def __new__(cls,
                pattern: str,
                time_model="dense",
                piecewise="constant",
                semantics="boolean",
                init_update=None):

        if time_model == "dense":
            if piecewise == "linear":
                if semantics == "boolean":
                    monitor = reelay.recipes.dense_timed_past_stl1_monitor(
                        pattern)
                    if init_update is not None:
                        monitor.init_update(init_update)
                    return monitor
                elif semantics == "robustness":
                    raise NotImplementedError(
                        "Robustness semantics is not available for dense time "
                        "piecewise linear signals. Please enable discrete "
                        "timed setting or use piecewise constant signals.")
                else:
                    raise AttributeError(
                        "Semantics must be 'boolean' or 'robustness'.")
            elif piecewise == "constant":
                if semantics == "boolean":
                    return reelay.recipes.dense_timed_past_stl0_monitor(
                        pattern)
                elif semantics == "robustness":
                    return reelay.recipes.dense_timed_past_rstl0_monitor(
                        pattern)
                else:
                    raise AttributeError(
                        "Semantics must be 'boolean' or 'robustness'.")
            else:
                raise AttributeError(
                    "Piecewise interpolation must be 'constant' or 'linear'.")

        elif time_model == "discrete":
            if semantics == "boolean":
                return reelay.recipes.discrete_timed_past_stl_monitor(pattern)
            elif semantics == "robustness":
                return reelay.recipes.discrete_timed_past_rstl_monitor(pattern)
            else:
                raise AttributeError(
                    "Semantics must be 'boolean' or 'robustness'.")

        elif time_model == "untimed" or time_model is None:
            if semantics == "boolean":
                return reelay.recipes.untimed_past_stl_monitor(pattern)
            elif semantics == "robustness":
                return reelay.recipes.untimed_past_rstl_monitor(pattern)
            else:
                raise AttributeError(
                    "Semantics must be 'boolean' or 'robustness'.")
        else:
            raise AttributeError(
                "Time model must be 'untimed', 'discrete', or 'dense'.")

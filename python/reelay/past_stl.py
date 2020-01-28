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
                piecewise="linear",
                enable_robustness=False,
                init_update=None):

        if time_model == "dense":
            if piecewise == "linear":
                if not enable_robustness:
                    monitor = reelay.recipes.dense_timed_past_stl1_monitor(
                        pattern)
                    if init_update is not None:
                        monitor.init_update(init_update)
                    return monitor
                else:
                    raise NotImplementedError(
                        "Robustness semantics is not available for dense time "
                        "piecewise linear signals. Please enable discrete "
                        "timed setting (time_model='discrete').")
            elif piecewise == "constant":
                if not enable_robustness:
                    return reelay.recipes.dense_timed_past_stl0_monitor(
                        pattern)
                else:
                    raise NotImplementedError(
                        "Robustness semantics is not available for dense time "
                        "piecewise constant signals. Please enable discrete "
                        "timed setting (time_model='discrete').")
            else:
                raise AttributeError(
                    "Piecewise interpolation must be 'constant' or 'linear'.")

        elif time_model == "discrete":
            if not enable_robustness:
                return reelay.recipes.discrete_timed_past_stl_monitor(pattern)
            else:
                return reelay.recipes.discrete_timed_past_rstl_monitor(pattern)

        elif time_model == "untimed" or time_model is None:
            if not enable_robustness:
                return reelay.recipes.untimed_past_stl_monitor(pattern)
            else:
                return reelay.recipes.untimed_past_rstl_monitor(pattern)

        else:
            raise AttributeError(
                "Time model must be 'untimed', 'discrete', or 'dense'.")

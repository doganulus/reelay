#
# Copyright (c) 2019-2020 Dogan Ulus
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
import reelay.recipes


class monitor(object):
    def __new__(cls, pattern: str, time_model="discrete", period=0):
        if time_model == "dense":
            return reelay.recipes.dense_timed_past_mtl_monitor(pattern)
        elif time_model == "discrete":
            return reelay.recipes.discrete_timed_past_mtl_monitor(pattern)
        elif time_model == "untimed" or time_model is None:
            return reelay.recipes.past_ltl_monitor(pattern)
        else:
            raise AttributeError("")

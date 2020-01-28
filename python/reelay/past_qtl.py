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
                time_model=None,
                event_type="dict[str,str]"):

        if time_model == "dense":
            raise NotImplementedError("")
        elif time_model == "discrete":
            raise NotImplementedError("")
        elif time_model is None and event_type == "dict[str,str]":
            return reelay.recipes.untimed_past_qtl_dict_monitor(pattern)
        elif time_model is None and event_type == "list[str]":
            return reelay.recipes.untimed_past_qtl_list_monitor(pattern)
        else:
            raise AttributeError("")

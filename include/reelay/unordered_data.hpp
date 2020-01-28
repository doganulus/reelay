/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "memory"
#include "string"
#include "functional"

#include "cudd.h"
#include "cuddObj.hh"

template <> struct std::hash<BDD> {
  std::size_t operator()(const BDD &k) const {
    return std::hash<DdNode*>{}(k.getNode());
  }
};

namespace reelay {

struct binding_manager;

using data_mgr_t = std::shared_ptr<binding_manager>;
using data_set_t = BDD;

struct binding_manager {

  using set_t = BDD;
  using mgr_t = Cudd;

  template<typename V = std::string>
  struct variable_t {
    set_t one;
    set_t cube;
    set_t other_cubes;
    set_t empty_slots;

    std::size_t n = 0;

    std::vector<set_t> bddvars;
    std::unordered_map<set_t, V> values = {};
    std::unordered_map<V, set_t> slots = {};

    variable_t(){}

    variable_t(const mgr_t &mgr, const std::vector<set_t> &vars)
        : one(mgr.bddOne()), cube(mgr.computeCube(vars)), empty_slots(~cube),
          bddvars(vars) {}

    std::size_t size(){return bddvars.size();}

    // set_t assign(const V &value) {
    //   if (slots.find(value) != slots.end()) {
    //     return slots[value];
    //   }
    //   auto slot = empty_slots.PickOneMinterm(bddvars);
    //   values[slot] = value;
    //   slots[value] = slot;
    //   empty_slots = ~slot * empty_slots; // Excludes slot from empty slots
    //   return slot;
    // }

    set_t assign(const V &value) {
      if (slots.find(value) != slots.end()) {
        return slots[value];
      }
      auto slot = make_minterm_of(n);
      n++;
      values[slot] = value;
      slots[value] = slot;
      return slot;
    }

    set_t make_minterm_of(std::size_t n) {
      std::size_t k = n; 
      set_t c = one;

      for(std::size_t i = 0; i < bddvars.size(); i++){
        if (k % 2 != 0) {
          c *= bddvars[i];
        } else {
          c *= ~bddvars[i];
        }
        k /= 2;
      }
      return c;
    }

    set_t erase(const V &value) {
      auto slot = slots[value];
      values.erase(slot);
      slots.erase(value);
      return slot;
    }

    V erase(const set_t &slot) {
      auto value = values[slot];
      slots.erase(value);
      values.erase(slot);
      return value;
    }

    set_t get_used_slots(const set_t &data_set) {
      auto used_slots = data_set.ExistAbstract(other_cubes) *
                        (~data_set).ExistAbstract(other_cubes);
      return used_slots;
    }
  };

  mgr_t cudd;

  std::unordered_map<std::string, variable_t<std::string>> variables = {};

  binding_manager() {
    cudd = Cudd(0, 0);
    cudd.AutodynDisable();
  }

  set_t one() { return cudd.bddOne(); }
  set_t zero() { return cudd.bddZero(); }

  void add_variable(std::string name, std::size_t nbits=20) {
    if (variables.find(name) == variables.end()) {
      std::vector<set_t> bddvars = {};
      for (std::size_t i = 0; i < nbits; i++) {
        bddvars.push_back(cudd.bddVar());
      }
      variables[name] = variable_t<std::string>(cudd, bddvars);
    }
  }

  set_t assign(std::string name, std::string value) {
    return variables[name].assign(value);
  }
};

} // namespace reelay

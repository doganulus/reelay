/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
 
#pragma once

#include <iostream>
#include <string>

#define PEGLIB_USE_STD_ANY 0
#include "reelay/third_party/cpp-peglib/peglib.h"

#include "reelay/networks.hpp"
#include "reelay/settings.hpp"

#include "reelay/parser/ptl_grammar.hpp"

namespace reelay {

template <class Setting> struct ptl_parser : ptl_grammar{

  using time_t = typename Setting::time_t;
  using input_t = typename Setting::input_t;
  using output_t = typename Setting::output_t;
  using function_t = typename Setting::function_t;

  using node_t = typename Setting::node_t;
  using state_t = typename Setting::state_t;
  using network_t = typename Setting::network_t;

  using node_ptr_t = typename Setting::node_ptr_t;
  using state_ptr_t = typename Setting::state_ptr_t;

  peg::parser parser;
  reelay::kwargs meta;

  std::vector<state_ptr_t> states = std::vector<state_ptr_t>();

  explicit ptl_parser(const reelay::kwargs &mm = reelay::kwargs())
      : meta(mm) {

    parser = peg::parser(grammar);
    parser.log = [](size_t line, size_t col, const std::string &msg) {
      std::cerr << line << ":" << col << ": " << msg << std::endl;
    };

    parser["ListProposition"].enter = [&](const char *s, size_t n, any &dt) {
      this->meta.insert({{"key", 0}});
    };

    parser["ListProposition"] = [&](const peg::SemanticValues &sv) {
      if (sv.size() > 1) {
        std::vector<node_ptr_t> args;
        for (size_t i = 0; i < sv.size(); i++) {
          this->meta["key"] = i;
          node_ptr_t child = reelay::any_cast<node_ptr_t>(sv[i]);
          args.push_back(child);
        }
        reelay::kwargs kw = {{"args", args}};
        kw.insert(meta.begin(), meta.end());
        auto expr = Setting::make_state("atomic_list", kw);

        this->states.push_back(expr);
        return std::static_pointer_cast<node_t>(expr);

      } else {
        node_ptr_t child = reelay::any_cast<node_ptr_t>(sv[0]);

        return child;
      }
    };

    parser["ListProposition"].leave =
        [&](const char *s, size_t n, size_t matchlen, any &value, any &dt) {
          this->meta.erase("key");
        };

    parser["SimpleRecordProposition"] = [&](const peg::SemanticValues &sv) {

      if (sv.size() > 1) {
        std::vector<node_ptr_t> args;
        for (size_t i = 0; i < sv.size(); i++) {
          node_ptr_t child = reelay::any_cast<node_ptr_t>(sv[i]);
          args.push_back(child);
        }
        reelay::kwargs kw = {{"args", args}};
        kw.insert(meta.begin(), meta.end());
        auto expr = Setting::make_state("atomic_map", kw);

        this->states.push_back(expr);
        return std::static_pointer_cast<node_t>(expr);

      } else {
        node_ptr_t child = reelay::any_cast<node_ptr_t>(sv[0]);

        return child;
      }
    };

    parser["NestedRecordProposition"] = [&](const peg::SemanticValues &sv) {
      auto path = reelay::any_cast<std::vector<std::string>>(sv[0]);

      std::vector<state_ptr_t> args;
      for (size_t i = 1; i < sv.size(); i++) {
        node_ptr_t child = reelay::any_cast<node_ptr_t>(sv[i]);
        args.push_back(std::static_pointer_cast<state_t>(child));
      }

      reelay::kwargs kw = {{"args", args}, {"path", path}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("atomic_nested", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["NestedAnyRecordProposition"] = [&](const peg::SemanticValues &sv) {
      auto path = reelay::any_cast<std::vector<std::string>>(sv[0]);

      std::vector<state_ptr_t> args;
      for (size_t i = 1; i < sv.size(); i++) {
        node_ptr_t child = reelay::any_cast<node_ptr_t>(sv[i]);
        args.push_back(std::static_pointer_cast<state_t>(child));
      }

      reelay::kwargs kw = {{"args", args}, {"path", path}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("atomic_nested_any", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["NestedAllRecordProposition"] = [&](const peg::SemanticValues &sv) {
      auto path = reelay::any_cast<std::vector<std::string>>(sv[0]);

      std::vector<state_ptr_t> args;
      for (size_t i = 1; i < sv.size(); i++) {
        node_ptr_t child = reelay::any_cast<node_ptr_t>(sv[i]);
        args.push_back(std::static_pointer_cast<state_t>(child));
      }

      reelay::kwargs kw = {{"args", args}, {"path", path}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("atomic_nested_all", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["KeyValueProp"] = [&](const peg::SemanticValues &sv) {
      auto keys = reelay::any_cast<std::vector<std::string>>(sv[0]);

      reelay::kwargs kw = {{"key", keys[0]}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("mapping_prop", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["KeyValuePairTrue"] = [&](const peg::SemanticValues &sv) {
      auto keys = reelay::any_cast<std::vector<std::string>>(sv[0]);

      reelay::kwargs kw = {{"key", keys[0]}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("mapping_true", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["KeyValuePairFalse"] = [&](const peg::SemanticValues &sv) {
      auto keys = reelay::any_cast<std::vector<std::string>>(sv[0]);

      reelay::kwargs kw = {{"key", keys[0]}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("mapping_false", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["KeyValuePairNumber"] = [&](const peg::SemanticValues &sv) {
      auto keys = reelay::any_cast<std::vector<std::string>>(sv[0]);
      auto cstr = reelay::any_cast<std::string>(sv[1]);

      reelay::kwargs kw = {{"key", keys[0]}, {"constant", cstr}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("mapping_number", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["KeyValuePairString"] = [&](const peg::SemanticValues &sv) {
      auto keys = reelay::any_cast<std::vector<std::string>>(sv[0]);
      auto cstr = reelay::any_cast<std::string>(sv[1]);

      reelay::kwargs kw = {{"key", keys[0]}, {"constant", cstr}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("mapping_string", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["KeyValuePairEQ"] = [&](const peg::SemanticValues &sv) {
      auto keys = reelay::any_cast<std::vector<std::string>>(sv[0]);
      auto cstr = reelay::any_cast<std::string>(sv[1]);

      reelay::kwargs kw = {{"key", keys[0]}, {"constant", cstr}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("mapping_eq", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["KeyValuePairNE"] = [&](const peg::SemanticValues &sv) {
      auto keys = reelay::any_cast<std::vector<std::string>>(sv[0]);
      auto cstr = reelay::any_cast<std::string>(sv[1]);

      reelay::kwargs kw = {{"key", keys[0]}, {"constant", cstr}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("mapping_ne", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["KeyValuePairGE"] = [&](const peg::SemanticValues &sv) {
      auto keys = reelay::any_cast<std::vector<std::string>>(sv[0]);
      auto cstr = reelay::any_cast<std::string>(sv[1]);

      reelay::kwargs kw = {{"key", keys[0]}, {"constant", cstr}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("mapping_ge", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["KeyValuePairGT"] = [&](const peg::SemanticValues &sv) {
      auto keys = reelay::any_cast<std::vector<std::string>>(sv[0]);
      auto cstr = reelay::any_cast<std::string>(sv[1]);

      reelay::kwargs kw = {{"key", keys[0]}, {"constant", cstr}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("mapping_gt", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["KeyValuePairLE"] = [&](const peg::SemanticValues &sv) {
      auto keys = reelay::any_cast<std::vector<std::string>>(sv[0]);
      auto cstr = reelay::any_cast<std::string>(sv[1]);

      reelay::kwargs kw = {{"key", keys[0]}, {"constant", cstr}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("mapping_le", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["KeyValuePairLT"] = [&](const peg::SemanticValues &sv) {
      auto keys = reelay::any_cast<std::vector<std::string>>(sv[0]);
      auto cstr = reelay::any_cast<std::string>(sv[1]);

      reelay::kwargs kw = {{"key", keys[0]}, {"constant", cstr}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("mapping_lt", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["KeyValuePairAnyValue"] = [&](const peg::SemanticValues &sv) {
      auto keys = reelay::any_cast<std::vector<std::string>>(sv[0]);

      reelay::kwargs kw = {{"key", keys[0]}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("mapping_any", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["KeyValuePairReference"] = [&](const peg::SemanticValues &sv) {
      auto keys = reelay::any_cast<std::vector<std::string>>(sv[0]);
      auto cstr = reelay::any_cast<std::string>(sv[1]);

      reelay::kwargs kw = {{"key", keys[0]}, {"constant", cstr}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("mapping_ref", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["FieldKey"] = [&](const peg::SemanticValues &sv) {
      auto keys = std::vector<std::string>();
      for (std::size_t i = 0; i < sv.size(); i++) {
        keys.push_back(reelay::any_cast<std::string>(sv[i]));
      }
      return keys;
    };

    parser["ListingTrue"] = [&](const peg::SemanticValues &sv) {
      auto index = reelay::any_cast<int>(this->meta["key"]);

      reelay::kwargs kw = {};
      kw.insert(meta.begin(), meta.end());

      auto expr = Setting::make_state("listing_true", kw);
      this->meta["key"] = index + 1; // Increase the index for the next item

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["ListingFalse"] = [&](const peg::SemanticValues &sv) {
      auto index = reelay::any_cast<int>(this->meta["key"]);

      reelay::kwargs kw = {};
      kw.insert(meta.begin(), meta.end());

      auto expr = Setting::make_state("listing_false", kw);
      this->meta["key"] = index + 1; // Increase the index for the next item

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["ListingNumber"] = [&](const peg::SemanticValues &sv) {
      auto cstr = reelay::any_cast<std::string>(sv[0]);
      auto index = reelay::any_cast<int>(this->meta["key"]);
      reelay::kwargs kw = {{"constant", cstr}};
      kw.insert(meta.begin(), meta.end());

      auto expr = Setting::make_state("listing_number", kw);
      this->meta["key"] = index + 1; // Increase the index for the next item

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["ListingGE"] = [&](const peg::SemanticValues &sv) {
      auto cstr = reelay::any_cast<std::string>(sv[0]);
      auto index = reelay::any_cast<int>(this->meta["key"]);
      reelay::kwargs kw = {{"constant", cstr}};
      kw.insert(meta.begin(), meta.end());

      auto expr = Setting::make_state("listing_ge", kw);
      this->meta["key"] = index + 1; // Increase the index for the next item

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["ListingGT"] = [&](const peg::SemanticValues &sv) {
      auto cstr = reelay::any_cast<std::string>(sv[0]);
      auto index = reelay::any_cast<int>(this->meta["key"]);
      reelay::kwargs kw = {{"constant", cstr}};
      kw.insert(meta.begin(), meta.end());

      auto expr = Setting::make_state("listing_gt", kw);
      this->meta["key"] = index + 1; // Increase the index for the next item

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["ListingLE"] = [&](const peg::SemanticValues &sv) {
      auto cstr = reelay::any_cast<std::string>(sv[0]);
      auto index = reelay::any_cast<int>(this->meta["key"]);
      reelay::kwargs kw = {{"constant", cstr}};
      kw.insert(meta.begin(), meta.end());

      auto expr = Setting::make_state("listing_le", kw);
      this->meta["key"] = index + 1; // Increase the index for the next item

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["ListingLT"] = [&](const peg::SemanticValues &sv) {
      auto cstr = reelay::any_cast<std::string>(sv[0]);
      auto index = reelay::any_cast<int>(this->meta["key"]);
      reelay::kwargs kw = {{"constant", cstr}};
      kw.insert(meta.begin(), meta.end());

      auto expr = Setting::make_state("listing_lt", kw);
      this->meta["key"] = index + 1; // Increase the index for the next item

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["ListingString"] = [&](const peg::SemanticValues &sv) {
      auto cstr = reelay::any_cast<std::string>(sv[0]);
      auto index = reelay::any_cast<int>(this->meta["key"]);
      reelay::kwargs kw = {{"constant", cstr}};
      kw.insert(meta.begin(), meta.end());

      auto expr = Setting::make_state("listing_string", kw);
      this->meta["key"] = index + 1; // Increase the index for the next item

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["ListingAnyValue"] = [&](const peg::SemanticValues &sv) {
      auto index = reelay::any_cast<int>(this->meta["key"]);
      reelay::kwargs kw = {};
      kw.insert(meta.begin(), meta.end());

      auto expr = Setting::make_state("listing_any", kw);
      this->meta["key"] = index + 1; // Increase the index for the next item

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["ListingReference"] = [&](const peg::SemanticValues &sv) {
      auto cstr = reelay::any_cast<std::string>(sv[0]);
      auto index = reelay::any_cast<int>(this->meta["key"]);
      reelay::kwargs kw = {{"constant", cstr}};
      kw.insert(meta.begin(), meta.end());

      auto expr = Setting::make_state("listing_ref", kw);
      this->meta["key"] = index + 1; // Increase the index for the next item

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["CustomPredicate"] = [&](const peg::SemanticValues &sv) {
      auto name = any_cast<std::string>(sv[0]);
      auto func = meta[name];

      reelay::kwargs kw = {{"function", func}};
      kw.insert(meta.begin(), meta.end()); 
      auto expr = Setting::make_state("predicate", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["ExistsExpr"] = [&](const peg::SemanticValues &sv) {
      auto vars = any_cast<std::vector<std::string>>(sv[0]);

      auto child = any_cast<node_ptr_t>(sv[1]);
      auto args = std::vector<node_ptr_t>({child});

      reelay::kwargs kw = {{"args", args}, {"vars", vars}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_node("exists", kw);

      return std::static_pointer_cast<node_t>(expr);
    };

    parser["ForallExpr"] = [&](const peg::SemanticValues &sv) {
      auto vars = any_cast<std::vector<std::string>>(sv[0]);

      auto child = any_cast<node_ptr_t>(sv[1]);
      auto args = std::vector<node_ptr_t>({child});

      reelay::kwargs kw = {{"args", args}, {"vars", vars}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_node("forall", kw);

      return std::static_pointer_cast<node_t>(expr);
    };

    parser["NotExpr"] = [&](const peg::SemanticValues &sv) {
      // Rule: NotExpr  <- LNOT Expression
      node_ptr_t child = any_cast<node_ptr_t>(sv[0]);
      auto args = std::vector<node_ptr_t>({child});

      reelay::kwargs kw = {{"args", args}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_node("negation", kw);

      return std::static_pointer_cast<node_t>(expr);
    };

    parser["Implicative"] = [&](const peg::SemanticValues &sv) {
      // Rule:
      if (sv.size() > 1) {
        std::vector<node_ptr_t> args;
        for (size_t i = 0; i < sv.size(); i++) {
          node_ptr_t child = any_cast<node_ptr_t>(sv[i]);
          args.push_back(child);
        }

        reelay::kwargs kw = {{"args", args}};
        kw.insert(meta.begin(), meta.end());
        auto expr = Setting::make_node("implication", kw);

        return std::static_pointer_cast<node_t>(expr);
      } else {
        node_ptr_t child = any_cast<node_ptr_t>(sv[0]);
        return child;
      }
    };

    parser["Disjunctive"] = [&](const peg::SemanticValues &sv) {
      // Rule:
      if (sv.size() > 1) {
        std::vector<node_ptr_t> args;
        for (size_t i = 0; i < sv.size(); i++) {
          node_ptr_t child = any_cast<node_ptr_t>(sv[i]);
          args.push_back(child);
        }

        reelay::kwargs kw = {{"args", args}};
        kw.insert(meta.begin(), meta.end());
        auto expr = Setting::make_node("disjunction", kw);

        return std::static_pointer_cast<node_t>(expr);
      } else {
        node_ptr_t child = any_cast<node_ptr_t>(sv[0]);
        return child;
      }
    };

    parser["Conjunctive"] = [&](const peg::SemanticValues &sv) {
      // Rule:
      if (sv.size() > 1) {
        std::vector<node_ptr_t> args;
        for (size_t i = 0; i < sv.size(); i++) {
          node_ptr_t child = any_cast<node_ptr_t>(sv[i]);
          args.push_back(child);
        }

        reelay::kwargs kw = {{"args", args}};
        kw.insert(meta.begin(), meta.end());
        auto expr = Setting::make_node("conjunction", kw);

        return std::static_pointer_cast<node_t>(expr);
      } else {
        node_ptr_t child = any_cast<node_ptr_t>(sv[0]);
        return child;
      }
    };

    parser["PrevExpr"] = [&](const peg::SemanticValues &sv) {
      // Rule:
      node_ptr_t child = any_cast<node_ptr_t>(sv[0]);
      auto args = std::vector<node_ptr_t>({child});

      reelay::kwargs kw = {{"args", args}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("previous", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["OnceExpr"] = [&](const peg::SemanticValues &sv) {
      // Rule:
      node_ptr_t child = any_cast<node_ptr_t>(sv[0]);
      auto args = std::vector<node_ptr_t>({child});

      reelay::kwargs kw = {{"args", args}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("past_sometime", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["TimedOnceExpr"] = [&](const peg::SemanticValues &sv) {
      std::pair<float, float> bound = any_cast<std::pair<float, float>>(sv[0]);
      node_ptr_t child = any_cast<node_ptr_t>(sv[1]);

      time_t lbound = std::get<0>(bound);
      time_t ubound = std::get<1>(bound);
      auto args = std::vector<node_ptr_t>({child});

      reelay::kwargs kw = {{"args", args},
                           {"lbound", lbound},
                           {"ubound", ubound}};
      kw.insert(meta.begin(), meta.end());
      state_ptr_t expr;
      if (ubound > 0) {
        expr = Setting::make_state("past_sometime_bounded", kw);
      } else {
        expr = Setting::make_state("past_sometime_bounded_half", kw);
      }

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["HistExpr"] = [&](const peg::SemanticValues &sv) {
      node_ptr_t child = any_cast<node_ptr_t>(sv[0]);
      auto args = std::vector<node_ptr_t>({child});

      reelay::kwargs kw = {{"args", args}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("past_always", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["TimedHistExpr"] = [&](const peg::SemanticValues &sv) {
      std::pair<float, float> bound = any_cast<std::pair<float, float>>(sv[0]);
      node_ptr_t child = any_cast<node_ptr_t>(sv[1]);

      time_t lbound = std::get<0>(bound);
      time_t ubound = std::get<1>(bound);
      auto args = std::vector<node_ptr_t>({child});

      reelay::kwargs kw = {{"args", args},
                           {"lbound", lbound},
                           {"ubound", ubound}};
      kw.insert(meta.begin(), meta.end());
      state_ptr_t expr;
      if (ubound > 0) {
        expr = Setting::make_state("past_always_bounded", kw);
      } else {
        expr = Setting::make_state("past_always_bounded_half", kw);
      }

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["SinceExpr"] = [&](const peg::SemanticValues &sv) {
      
      if (sv.size() == 3) {
        node_ptr_t left = any_cast<node_ptr_t>(sv[0]);
        std::pair<float, float> bound =
            any_cast<std::pair<float, float>>(sv[1]);
        node_ptr_t right = any_cast<node_ptr_t>(sv[2]);

        time_t lbound = std::get<0>(bound);
        time_t ubound = std::get<1>(bound);
        auto args = std::vector<node_ptr_t>({left, right});

        reelay::kwargs kw = {{"args", args},
                             {"lbound", lbound},
                             {"ubound", ubound}};
        kw.insert(meta.begin(), meta.end());

        state_ptr_t expr;
        if (ubound > 0){
          expr = Setting::make_state("since_bounded", kw);
        } else {
          expr = Setting::make_state("since_bounded_half", kw);
        }

        this->states.push_back(expr);
        return std::static_pointer_cast<node_t>(expr);

      } else if (sv.size() == 2) {
        node_ptr_t left = any_cast<node_ptr_t>(sv[0]);
        node_ptr_t right = any_cast<node_ptr_t>(sv[1]);
        auto args = std::vector<node_ptr_t>({left, right});

        reelay::kwargs kw = {{"args", args}};
        kw.insert(meta.begin(), meta.end());
        auto expr = Setting::make_state("since", kw);

        this->states.push_back(expr);
        return std::static_pointer_cast<node_t>(expr);

      } else {
        node_ptr_t child = any_cast<node_ptr_t>(sv[0]);
        return child;
      }
    };

    parser["FullBound"] = [](const peg::SemanticValues &sv) {
      float l = std::stof(any_cast<std::string>(sv[0]));
      float u = std::stof(any_cast<std::string>(sv[1]));
      return std::make_pair(l, u);
    };

    parser["LowerBound"] = [](const peg::SemanticValues &sv) {
      float l = std::stof(any_cast<std::string>(sv[0]));
      return std::make_pair(l, 0.0f);
    };

    parser["UpperBound"] = [](const peg::SemanticValues &sv) {
      float u = std::stof(any_cast<std::string>(sv[0]));
      return std::make_pair(0.0f, u);
    };

    parser["NonEmptyVarList"] = [&](const peg::SemanticValues &sv) {
      auto vlist = std::vector<std::string>();

      for (std::size_t i = 0; i < sv.size(); i++) {
        auto child = reelay::any_cast<std::string>(sv[i]);
        vlist.push_back(child);
      }

      return vlist;
    };

    parser["PathKey"] = [](const peg::SemanticValues &sv) {
      std::vector<std::string> path;
      for (std::size_t i = 0; i < sv.size(); i++) {
        path.push_back(any_cast<std::string>(sv[i]));
      }
      return path;
    };

    parser["Name"] = [](const peg::SemanticValues &sv) { return sv.token(); };
    parser["Number"] = [](const peg::SemanticValues &sv) { return sv.token(); };
    parser["SQString"] = [](const peg::SemanticValues &sv) {
      return sv.token();
    };
    parser["DQString"] = [](const peg::SemanticValues &sv) {
      return sv.token();
    };

    parser.enable_packrat_parsing(); // Enable packrat parsing.
  }

  std::shared_ptr<network_t> parse(const std::string &pattern) {
    node_ptr_t output_func;
    parser.parse(pattern.c_str(), output_func);

    auto network = std::make_shared<network_t>(output_func, states);
    return network;
  }
};

} // namespace reelay
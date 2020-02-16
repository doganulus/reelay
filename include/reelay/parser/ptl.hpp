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

namespace reelay {

template <class Setting> struct ptl_parser {

  using time_t = typename Setting::time_t;
  using input_t = typename Setting::input_t;
  using output_t = typename Setting::output_t;
  using function_t = typename Setting::function_t;

  using node_t = typename Setting::node_t;
  using state_t = typename Setting::state_t;
  using network_t = typename Setting::network_t;

  using node_ptr_t = typename Setting::node_ptr_t;
  using state_ptr_t = typename Setting::state_ptr_t;

  static constexpr auto grammar = R"(
    Expression  <- ExistsExpr / ForallExpr / Implicative
    ExistsExpr  <- EXISTS '[' NonEmptyVarList ']' '.' Implicative 
    ForallExpr  <- FORALL '[' NonEmptyVarList ']' '.' Implicative
    Implicative <- Disjunctive (LIMPLIES Disjunctive)?
    Disjunctive <- Conjunctive (LOR Conjunctive)*
    Conjunctive <- SinceExpr (LAND SinceExpr)*
    SinceExpr <- Unary (SINCE Bound? Unary)?
    Unary <- NotExpr / PrevExpr / TimedOnceExpr / OnceExpr / TimedHistExpr / HistExpr / Atom / '(' Expression ')'
    NotExpr  <- LNOT Atom / LNOT '(' Expression ')'
    PrevExpr <- PREV Atom / PREV '(' Expression ')'
    OnceExpr <- ONCE Atom / ONCE '(' Expression ')'
    HistExpr <- HIST Atom / HIST '(' Expression ')'
    TimedOnceExpr <- ONCE Bound Atom / ONCE Bound '(' Expression ')'
    TimedHistExpr <- HIST Bound Atom / HIST Bound '(' Expression ')'
    Atom <- CustomPredicate / BasicPredicateLE / BasicPredicateLT / BasicPredicateGE / BasicPredicateGT / ListProposition / RecordProposition / Proposition
            
    Proposition <- Name
    BasicPredicateLE <- Name "<=" Number
    BasicPredicateLT <- Name "<" Number
    BasicPredicateGE <- Name ">=" Number
    BasicPredicateGT <- Name ">" Number
    CustomPredicate <- '$' Name

    ListProposition <- LBRACE NonEmptyValueList RBRACE
    RecordProposition <- LCURLY NonEmptyKeyValuePairs RCURLY
    EventRecordProposition <- EVENT LCURLY NonEmptyKeyValuePairs RCURLY

    NonEmptyVarList <- Name (COMMA Name)*
    NonEmptyValueList <- FieldValue (COMMA FieldValue)*
    NonEmptyKeyValuePairs <- KeyValuePair (COMMA KeyValuePair)*

    FieldValueTrue <- TRUE
    FieldValueFalse <- FALSE
    FieldValueString <- SQString / DQString / Name
    FieldValueNumber <- Number
    FieldKey <- Name (DOT Name)*
    FieldValue <- FieldValueTrue / FieldValueFalse / FieldValueString / FieldValueNumber / VariableRef / AnyValue

    KeyValuePair <- KeyValuePairST / KeyValuePairEQ / KeyValuePairNE / KeyValuePairLT / KeyValuePairLE / KeyValuePairGT / KeyValuePairGE / KeyValueProp
    
    KeyValueProp <- FieldKey
    KeyValuePairST <- FieldKey ':' FieldValue
    KeyValuePairEQ <- FieldKey EQ Number
    KeyValuePairNE <- FieldKey NE Number
    KeyValuePairLT <- FieldKey LT Number
    KeyValuePairLE <- FieldKey LE Number
    KeyValuePairGT <- FieldKey GT Number
    KeyValuePairGE <- FieldKey GE Number

    AnyValue <- STAR
    VariableRef <- STAR Name
  
    Bound <- FullBound / LowerBound / UpperBound
    FullBound <-  "[" Number ":" Number "]"
    LowerBound <- "[" Number ":" 'inf'? "]"
    UpperBound <- "["        ":" Number "]"
        
    ~EVENT <- < '@' / 'event' >

    ~PREV   <- < 'Y' / 'pre' >
    ~HIST  <- < 'H' / 'historically' >
    ~ONCE  <- < 'P' / 'once' >
    ~SINCE <- < 'S' / 'since' >
            
    ~LOR      <- < "||" / 'or' >
    ~LAND     <- < "&&" / 'and' >
    ~LNOT     <- < "!"  / 'not' >
    ~LIMPLIES <- < "->" / 'implies' >
    ~EXISTS <- < 'E' / 'exists' >
    ~FORALL <- < 'A' / 'forall' >
    ~COMMA <- < ',' >
    ~LPARAM <- < '(' >
    ~RPARAM <- < ')' >
    ~LBRACE <- < '[' >
    ~RBRACE <- < ']' >
    ~LCURLY <- < '{' >
    ~RCURLY <- < '}' >

    ~LT <- < '<' >
    ~LE <- < '<=' >
    ~GT <- < '>' >
    ~GE <- < '>=' >
    ~EQ <- < '==' >
    ~NE <- < '!=' >

    ~DOT <- < '.' >
    ~STAR <- < '*' >
    ~AMSAND <- < '&' >
    ~DOLLAR <- < '$' >
    ~SQUARE <- < '#' >
    ~SQ <- < "'" >
    ~DQ <- < '"' >
    
    Name   <- <[_a-zA-Z][_a-zA-Z0-9]*>
    SQString <- SQ <[^']*> SQ
    DQString <- DQ <[^"]*> DQ
    Number <- <'-'? [0-9]+ ('.' [0-9]+)?>
    ~TRUE  <- < 'true'>
    ~FALSE <- < 'false'>    
            
    %whitespace <- [  \t\r\n]*
    )";

  peg::parser parser;
  reelay::kwargs meta;

  std::vector<state_ptr_t> states = std::vector<state_ptr_t>();

  explicit ptl_parser(const reelay::kwargs &mm = reelay::kwargs())
      : meta(mm) {

    parser = peg::parser(grammar);
    parser.log = [](size_t line, size_t col, const std::string &msg) {
      std::cerr << line << ":" << col << ": " << msg << std::endl;
    };

    parser["Proposition"] = [&](const peg::SemanticValues &sv) {
      auto name = any_cast<std::string>(sv[0]);

      reelay::kwargs kw = {{"name", name}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("proposition", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["ListProposition"] = [&](const peg::SemanticValues &sv) {
      auto fields =
          any_cast<std::vector<std::pair<std::string, std::string>>>(sv[0]);
      reelay::kwargs kw = {{"fields", fields}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("listing", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["RecordProposition"] = [&](const peg::SemanticValues &sv) {
      auto fields = any_cast<std::vector<
          std::pair<std::string, std::pair<std::string, std::string>>>>(sv[0]);

      reelay::kwargs kw = {{"fields", fields}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("record", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["NonEmptyVarList"] = [&](const peg::SemanticValues &sv) {
      auto vlist = std::vector<std::string>();

      for (std::size_t i = 0; i < sv.size(); i++) {
        auto child = any_cast<std::string>(sv[i]);
        vlist.push_back(child);
      }

      return vlist;
    };

    parser["NonEmptyValueList"] = [&](const peg::SemanticValues &sv) {
      auto vlist = std::vector<std::pair<std::string, std::string>>();

      for (std::size_t i = 0; i < sv.size(); i++) {
        auto child = any_cast<std::pair<std::string, std::string>>(sv[i]);
        vlist.push_back(child);
      }

      return vlist;
    };

    parser["NonEmptyKeyValuePairs"] = [&](const peg::SemanticValues &sv) {
      auto keyvals = std::vector<
          std::pair<std::string, std::pair<std::string, std::string>>>();

      for (std::size_t i = 0; i < sv.size(); i++) {
        auto child = any_cast<
            std::pair<std::string, std::pair<std::string, std::string>>>(sv[i]);
        keyvals.push_back(child);
      }

      return keyvals;
    };

    parser["FieldValueTrue"] = [&](const peg::SemanticValues &sv) {
      return std::pair<std::string, std::string>("bool", "true");
    };

    parser["FieldValueFalse"] = [&](const peg::SemanticValues &sv) {
      return std::pair<std::string, std::string>("bool", "false");
    };

    parser["FieldValueNumber"] = [&](const peg::SemanticValues &sv) {
      return std::pair<std::string, std::string>("number",
                                                 any_cast<std::string>(sv[0]));
    };

    parser["FieldValueString"] = [&](const peg::SemanticValues &sv) {
      return std::pair<std::string, std::string>("string",
                                                 any_cast<std::string>(sv[0]));
    };

    parser["VariableRef"] = [&](const peg::SemanticValues &sv) {
      return std::pair<std::string, std::string>("variable_ref",
                                                 any_cast<std::string>(sv[0]));
    };

    parser["AnyValue"] = [&](const peg::SemanticValues &) {
      return std::pair<std::string, std::string>("ignore_field","");
    };

    parser["FieldKey"] = [&](const peg::SemanticValues &sv) {
      auto keys = std::vector<std::string>();
      for (std::size_t i = 0; i < sv.size(); i++) {
        keys.push_back(any_cast<std::string>(sv[i]));
      }
      return keys;
    };

    parser["KeyValueProp"] = [&](const peg::SemanticValues &sv) {
      auto key_path = any_cast<std::vector<std::string>>(sv[0]);
      return std::pair<std::string, std::pair<std::string, std::string>>(
          key_path[0], std::pair<std::string, std::string>("bool", "true"));
    };

    parser["KeyValuePairST"] = [&](const peg::SemanticValues &sv) {
      auto key_path = any_cast<std::vector<std::string>>(sv[0]);
      auto value = any_cast<std::pair<std::string, std::string>>(sv[1]);
      return std::pair<std::string, std::pair<std::string, std::string>>(
          key_path[0], value);
    };

    parser["KeyValuePairGE"] = [&](const peg::SemanticValues &sv) {
      auto key_path = any_cast<std::vector<std::string>>(sv[0]);
      auto number_str = any_cast<std::string>(sv[1]);
      return std::pair<std::string, std::pair<std::string, std::string>>(
          key_path[0], std::pair<std::string, std::string>("ge", number_str));
    };

    parser["KeyValuePairGT"] = [&](const peg::SemanticValues &sv) {
      auto key_path = any_cast<std::vector<std::string>>(sv[0]);
      auto number_str = any_cast<std::string>(sv[1]);
      return std::pair<std::string, std::pair<std::string, std::string>>(
          key_path[0], std::pair<std::string, std::string>("gt", number_str));
    };

    parser["KeyValuePairLE"] = [&](const peg::SemanticValues &sv) {
      auto key_path = any_cast<std::vector<std::string>>(sv[0]);
      auto number_str = any_cast<std::string>(sv[1]);
      return std::pair<std::string, std::pair<std::string, std::string>>(
          key_path[0], std::pair<std::string, std::string>("le", number_str));
    };

    parser["KeyValuePairLT"] = [&](const peg::SemanticValues &sv) {
      auto key_path = any_cast<std::vector<std::string>>(sv[0]);
      auto number_str = any_cast<std::string>(sv[1]);
      return std::pair<std::string, std::pair<std::string, std::string>>(
          key_path[0], std::pair<std::string, std::string>("lt", number_str));
    };

    parser["BasicPredicateLT"] = [&](const peg::SemanticValues &sv) {
      auto name = any_cast<std::string>(sv[0]);
      auto constant = std::stof(any_cast<std::string>(sv[1]));

      reelay::kwargs kw = {{"name", name}, {"constant", constant}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("lt", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["BasicPredicateLE"] = [&](const peg::SemanticValues &sv) {
      auto name = any_cast<std::string>(sv[0]);
      auto constant = std::stof(any_cast<std::string>(sv[1]));

      reelay::kwargs kw = {{"name", name}, {"constant", constant}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("le", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["BasicPredicateGT"] = [&](const peg::SemanticValues &sv) {
      auto name = any_cast<std::string>(sv[0]);
      auto constant = std::stof(any_cast<std::string>(sv[1]));

      reelay::kwargs kw = {{"name", name}, {"constant", constant}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("gt", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["BasicPredicateGE"] = [&](const peg::SemanticValues &sv) {
      auto name = any_cast<std::string>(sv[0]);
      auto constant = std::stof(any_cast<std::string>(sv[1]));

      reelay::kwargs kw = {{"name", name}, {"constant", constant}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("ge", kw);

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
      // Rule:
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
      // Rule:
      node_ptr_t child = any_cast<node_ptr_t>(sv[0]);
      auto args = std::vector<node_ptr_t>({child});

      reelay::kwargs kw = {{"args", args}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_state("past_always", kw);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["TimedHistExpr"] = [&](const peg::SemanticValues &sv) {
      // Rule:
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
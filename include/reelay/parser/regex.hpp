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
#include "reelay/third_party/peglib.h"

#include "reelay/networks.hpp"
#include "reelay/settings.hpp"

namespace reelay {

template <class Setting> struct regex_parser {

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
    Expression  <- Disjunctive
    Disjunctive <- Conjunctive (REG_OR Conjunctive)*
    Conjunctive <- SequenceExpr (REG_AND SequenceExpr)*
    SequenceExpr <- RegularUnary (REG_CAT? RegularUnary)*
    
    RegularUnary <- RegularStar / RegularPlus / RegularOption / RegularBounded /BooleanExpression / RegularGrouping
    RegularStar <- BooleanExpression STAR / LPARAM Expression RPARAM STAR
    RegularPlus <- BooleanExpression PLUS / LPARAM Expression RPARAM PLUS
    RegularOption <- BooleanExpression QMARK / LPARAM Expression RPARAM QMARK
    RegularBounded <- BooleanExpression TimeBound / LPARAM Expression RPARAM TimeBound
    RegularGrouping <- LPARAM Expression RPARAM
    
    BooleanExpression  <- BoolDisjunctive
    BoolDisjunctive <- BoolConjunctive (LOR BoolConjunctive)*
    BoolConjunctive <- BooleanUnary (LAND BooleanUnary)*
    
    BooleanUnary <- BoolNegative / BooleanAtom / LPARAM BooleanExpression RPARAM
    BoolNegative <- LNOT BooleanAtom / LNOT LPARAM BooleanExpression RPARAM
    BooleanAtom <- RecordProposition / AnyRecord

    AnyRecord <- LCURLY STAR RCURLY
    RecordProposition <- LCURLY NonEmptyKeyValuePairs RCURLY

    NonEmptyKeyValuePairs <- KeyValuePair (COMMA KeyValuePair)*

    FieldKey <- Name (DOT Name)*
    FieldValue <- Bool / SQString / DQString / Name / Number / VariableRef / AnyValue 
    KeyValuePair <- KeyValuePairST / KeyValuePairEQ / KeyValuePairNE / KeyValuePairLT / KeyValuePairLE / KeyValuePairGT / KeyValuePairGE
    
    KeyValuePairST <- FieldKey ':' FieldValue
    KeyValuePairEQ <- FieldKey '==' Number
    KeyValuePairNE <- FieldKey '!=' Number
    KeyValuePairLT <- FieldKey '<' Number
    KeyValuePairLE <- FieldKey '<=' Number
    KeyValuePairGT <- FieldKey '>' Number
    KeyValuePairGE <- FieldKey '>=' Number
    
    AnyValue <- STAR
    VariableRef <- STAR Name

    TimeBound <- FullBound / LowerBound / UpperBound
    FullBound <-  "[" Number ":" Number "]"
    LowerBound <- "[" Number ":" 'inf'? "]"
    UpperBound <- "["        ":" Number "]"
    
    ~REG_OR   <- < "|" / 'or'>
    ~REG_AND  <- < "&" / 'and'>
    ~REG_CAT  <- < ';' >
    
    ~LOR      <- < "||" >
    ~LAND     <- < "&&" >
    ~LNOT     <- < "!" >
    ~LIMPLIES <- < "->" / 'implies' >

    ~COMMA <- < ',' >
    ~LPARAM <- < '(' >
    ~RPARAM <- < ')' >
    ~LBRACE <- < '[' >
    ~RBRACE <- < ']' >
    ~LCURLY <- < '{' >
    ~RCURLY <- < '}' >
    
    ~DOT <- < '.' >
    ~THREEDOT <- < '...' >
    ~STAR <- < '*' >
    ~PLUS <- < '+' >
    ~QMARK <- < '?' >
    ~AMSAND <- < '&' >
    ~DOLLAR <- < '$' >
    ~SQUARE <- < '#' >
    
    Name   <- <[_a-zA-Z][_a-zA-Z0-9]*>
    SQString <- "'" < [^']* > "'"
    DQString <- '"' < [^"]* > '"'
    Number <- <'-'? [0-9]+ ('.' [0-9]+)?>
    Bool <- TRUE / FALSE
    ~TRUE  <- < 'true'>
    ~FALSE <- < 'false'>      

    %whitespace <- [  \t\r\n]*
    )";

  peg::parser parser;
  reelay::kwargs meta;

  std::vector<state_ptr_t> states = std::vector<state_ptr_t>();

  explicit regex_parser(const reelay::kwargs &kw = reelay::kwargs())
      : meta(kw) {

    parser = peg::parser(grammar);
    parser.log = [](size_t line, size_t col, const std::string &msg) {
      std::cerr << line << ":" << col << ": " << msg << std::endl;
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

    parser["VariableRef"] = [&](const peg::SemanticValues &sv) {
      return std::pair<std::string, std::string>("variable_ref",
                                                 any_cast<std::string>(sv[0]));
    };

    parser["UnnamedRef"] = [&](const peg::SemanticValues &) {
      return std::pair<std::string, std::string>("ignore_field","");
    };

    parser["FieldKey"] = [&](const peg::SemanticValues &sv) {
      auto keys = std::vector<std::string>();
      for (std::size_t i = 0; i < sv.size(); i++) {
        keys.push_back(any_cast<std::string>(sv[i]));
      }
      return keys;
    };

    parser["KeyValuePair"] = [&](const peg::SemanticValues &sv) {
      auto key_path = any_cast<std::vector<std::string>>(sv[0]);
      auto value = any_cast<std::pair<std::string, std::string>>(sv[1]);
      auto key = key_path[0];
      for (std::size_t i = 1; i < key_path.size(); i++) {
        key += '/'+ key_path[i];
      }
      return std::pair<std::string, std::pair<std::string, std::string>>(key, value);
    };

    parser["BoolNegative"] = [&](const peg::SemanticValues &sv) {
      node_ptr_t child = any_cast<node_ptr_t>(sv[0]);
      auto args = std::vector<node_ptr_t>({child});

      reelay::kwargs kw = {{"args", args}};
      kw.insert(meta.begin(), meta.end());
      auto expr = Setting::make_node("negation", kw);

      return std::static_pointer_cast<node_t>(expr);
    };

    parser["BoolDisjunctive"] = [&](const peg::SemanticValues &sv) {
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

    parser["BoolConjunctive"] = [&](const peg::SemanticValues &sv) {
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
    parser["SQString"] = [](const peg::SemanticValues &sv) {
      return sv.token();
    };
    parser["DQString"] = [](const peg::SemanticValues &sv) {
      return sv.token();
    };
    parser["Number"] = [](const peg::SemanticValues &sv) { return sv.token(); };
    parser["Bool"] = [](const peg::SemanticValues &sv) { return sv.token(); };

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
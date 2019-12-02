/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include <iostream>
#include <string>

#include "reelay/third_party/peglib.h"

#include "reelay/networks.hpp"
#include "reelay/settings.hpp"

namespace reelay {

template <class Setting>
struct ptl_parser {

  using input_t = typename Setting::input_t;
  using output_t = typename Setting::output_t;
  using function_t = typename Setting::function_t;

  using node_t = typename Setting::node_t;
  using state_t = typename Setting::state_t;
  using network_t = typename Setting::network_t;

  static constexpr auto grammar = R"(
    Expression  <- Implicative 
    Implicative <- Disjunctive (LIMPLIES Disjunctive)?
    Disjunctive <- Conjunctive (LOR Conjunctive)*
    Conjunctive <- SinceExpr (LAND SinceExpr)*
    SinceExpr <- Unary (SINCE Bound? Unary)?

    Unary <- NotExpr / TimedOnceExpr / OnceExpr / TimedHistExpr / HistExpr / Atom / '(' Expression ')'
    NotExpr  <- LNOT Expression
    OnceExpr <- ONCE Expression
    HistExpr <- HIST Expression
    TimedOnceExpr <- ONCE Bound Expression
    TimedHistExpr <- HIST Bound Expression

    Atom <- CustomPredicate / BasicPredicateLE / BasicPredicateLT / BasicPredicateGE / BasicPredicateGT / Proposition
            
    Proposition <- Name
    BasicPredicateLE <- Name "<=" Number
    BasicPredicateLT <- Name "<" Number
    BasicPredicateGE <- Name ">=" Number
    BasicPredicateGT <- Name ">" Number
    CustomPredicate <- '$' Name
  
    Bound <- FullBound / LowerBound / UpperBound
    FullBound <-  "[" Number ":" Number "]"
    LowerBound <- "[" Number ":" 'inf'? "]"
    UpperBound <- "["        ":" Number "]"
        
    ~PRE   <- < 'Y' / 'pre' >
    ~HIST  <- < 'H' / 'historically' >
    ~ONCE  <- < 'P' / 'once' >
    ~SINCE <- < 'S' / 'since' >
            
    ~LOR      <- < "||" / 'or' >
    ~LAND     <- < "&&" / 'and' >
    ~LNOT     <- < "!"  / 'not' >
    ~LIMPLIES <- < "->" / 'implies' >
            
    Name   <- <[_a-zA-Z][_a-zA-Z0-9]*>
    Number <- <'-'? [0-9]+ ('.' [0-9]+)?>
            
    %whitespace <- [  \t\r\n]*
    )";

  peg::parser parser;

  std::vector<std::shared_ptr<state_t>> states =
    std::vector<std::shared_ptr<state_t>>();

  std::map<std::string, function_t> predicates;

  explicit ptl_parser(const std::map<std::string, function_t> &pp =
                          std::map<std::string, function_t>())
      : predicates(pp) {

    parser = peg::parser(grammar);
    parser.log = [](size_t line, size_t col, const std::string &msg) {
      std::cerr << line << ":" << col << ": " << msg << std::endl;
    };

    parser["Proposition"] = [&](const peg::SemanticValues &sv) {
      auto name = sv[0].get<std::string>();
      auto expr = Setting::make_proposition(name);
      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["BasicPredicateLT"] = [&](const peg::SemanticValues &sv) {

      auto name = sv[0].get<std::string>();
      auto constant = std::stof(sv[1].get<std::string>());

      auto expr = Setting::make_basic_predicate(name, "lt", constant);
      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["BasicPredicateLE"] = [&](const peg::SemanticValues &sv) {
      auto name = sv[0].get<std::string>();
      auto constant = std::stof(sv[1].get<std::string>());

      auto expr = Setting::make_basic_predicate(name, "le", constant);
      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["BasicPredicateGT"] = [&](const peg::SemanticValues &sv) {
      auto name = sv[0].get<std::string>();
      auto constant = std::stof(sv[1].get<std::string>());

      auto expr = Setting::make_basic_predicate(name, "gt", constant);
      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["BasicPredicateGE"] = [&](const peg::SemanticValues &sv) {
      auto name = sv[0].get<std::string>();
      auto constant = std::stof(sv[1].get<std::string>());

      auto expr = Setting::make_basic_predicate(name, "ge", constant);
      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["CustomPredicate"] = [&](const peg::SemanticValues &sv) {
      auto name = sv[0].get<std::string>();
      auto func = predicates[name];
      auto expr = Setting::make_predicate(func);

      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["Comparison"] = [](const peg::SemanticValues &sv) { return sv.token(); };

    parser["NotExpr"] = [&](const peg::SemanticValues &sv) {
      // Rule: NotExpr  <- LNOT Expression
      std::shared_ptr<node_t> child = sv[0].get<std::shared_ptr<node_t>>();
      auto args = std::vector<std::shared_ptr<node_t>>({child});
      auto expr = Setting::make_node("negation", args);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["Implicative"] = [&](const peg::SemanticValues &sv) {
      // Rule:
      if (sv.size() > 1) {
        std::vector<std::shared_ptr<node_t>> args;
        for (size_t i = 0; i < sv.size(); i++) {
          std::shared_ptr<node_t> child = sv[i].get<std::shared_ptr<node_t>>();
          args.push_back(child);
        }
        auto expr = Setting::make_node("implication", args);
        return std::static_pointer_cast<node_t>(expr);
      } else {
        std::shared_ptr<node_t> child = sv[0].get<std::shared_ptr<node_t>>();
        return child;
      }
    };

    parser["Disjunctive"] = [&](const peg::SemanticValues &sv) {
      // Rule:
      if (sv.size() > 1) {
        std::vector<std::shared_ptr<node_t>> args;
        for (size_t i = 0; i < sv.size(); i++) {
          std::shared_ptr<node_t> child = sv[i].get<std::shared_ptr<node_t>>();
          args.push_back(child);
        }
        auto expr = Setting::make_node("disjunction", args);
        return std::static_pointer_cast<node_t>(expr);
      } else {
        std::shared_ptr<node_t> child = sv[0].get<std::shared_ptr<node_t>>();
        return child;
      }
    };

    parser["Conjunctive"] = [&](const peg::SemanticValues &sv) {
      // Rule: 
      if(sv.size() > 1){
        std::vector<std::shared_ptr<node_t>> args;
        for (size_t i = 0; i < sv.size(); i++) {
          std::shared_ptr<node_t> child = sv[i].get<std::shared_ptr<node_t>>();
          args.push_back(child);
        }
        auto expr = Setting::make_node("conjunction", args);
        return std::static_pointer_cast<node_t>(expr);
      } else {
        std::shared_ptr<node_t> child = sv[0].get<std::shared_ptr<node_t>>();
        return child;
      }
    };

    parser["OnceExpr"] = [&](const peg::SemanticValues &sv) {
      // Rule:
      std::shared_ptr<node_t> child = sv[0].get<std::shared_ptr<node_t>>();
      auto args = std::vector<std::shared_ptr<node_t>>({child});
      auto expr = Setting::make_state("past_sometime", args);
      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["TimedOnceExpr"] = [&](const peg::SemanticValues &sv) {
      // Rule:
      std::pair<float, float> bound = sv[0].get<std::pair<float, float>>();
      std::shared_ptr<node_t> child = sv[1].get<std::shared_ptr<node_t>>();

      auto args = std::vector<std::shared_ptr<node_t>>({child});
      auto expr = Setting::make_state("past_sometime_bounded", args, bound);
      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["HistExpr"] = [&](const peg::SemanticValues &sv) {
      // Rule:
      std::shared_ptr<node_t> child = sv[0].get<std::shared_ptr<node_t>>();
      auto args = std::vector<std::shared_ptr<node_t>>({child});
      auto expr = Setting::make_state("past_always", args);
      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["TimedHistExpr"] = [&](const peg::SemanticValues &sv) {
      // Rule:
      std::pair<float, float> bound = sv[0].get<std::pair<float, float>>();
      std::shared_ptr<node_t> child = sv[1].get<std::shared_ptr<node_t>>();

      auto args = std::vector<std::shared_ptr<node_t>>({child});
      auto expr = Setting::make_state("past_always_bounded", args, bound);
      this->states.push_back(expr);
      return std::static_pointer_cast<node_t>(expr);
    };

    parser["SinceExpr"] = [&](const peg::SemanticValues &sv) {
      // Rule:
      if (sv.size() == 3) {
        std::shared_ptr<node_t> left = sv[0].get<std::shared_ptr<node_t>>();
        std::pair<float, float> bound = sv[1].get<std::pair<float, float>>();
        std::shared_ptr<node_t> right = sv[2].get<std::shared_ptr<node_t>>();
        auto args = std::vector<std::shared_ptr<node_t>>({left, right});
        auto expr = Setting::make_state("since_bounded", args, bound);
        this->states.push_back(expr);
        return std::static_pointer_cast<node_t>(expr);

      } else if (sv.size() == 2) {
        std::shared_ptr<node_t> left = sv[0].get<std::shared_ptr<node_t>>();
        std::shared_ptr<node_t> right = sv[1].get<std::shared_ptr<node_t>>();
        auto args = std::vector<std::shared_ptr<node_t>>({left, right});
        auto expr = Setting::make_state("since", args);
        this->states.push_back(expr);
        return std::static_pointer_cast<node_t>(expr);

      } else {
        std::shared_ptr<node_t> child = sv[0].get<std::shared_ptr<node_t>>();
        return child;
      }
    };

    parser["TimedSinceExpr"] = [&](const peg::SemanticValues &sv) {
      // Rule:
      if (sv.size() > 1) {

      } else {
        std::shared_ptr<node_t> child = sv[0].get<std::shared_ptr<node_t>>();
        return child;
      }
    };

    parser["FullBound"] = [](const peg::SemanticValues &sv) {
      float l = std::stof(sv[0].get<std::string>());
      float u = std::stof(sv[1].get<std::string>());
      return std::make_pair(l,u);
    };

    parser["LowerBound"] = [](const peg::SemanticValues &sv) {
      float l = std::stof(sv[0].get<std::string>());
      return std::make_pair(l, 0.0f);
    };

    parser["UpperBound"] = [](const peg::SemanticValues &sv) {
      float u = std::stof(sv[0].get<std::string>());
      return std::make_pair(0.0f, u);
    };

    parser["Name"] = [](const peg::SemanticValues &sv) {
      return sv.token();
    };
    parser["Number"] = [](const peg::SemanticValues &sv) {
      return sv.token();
    };

    parser.enable_packrat_parsing(); // Enable packrat parsing.
  }

  std::shared_ptr<network_t> parse(const std::string &formula) {
    std::shared_ptr<node_t> output_func;
    parser.parse(formula.c_str(), output_func);

    auto network = std::make_shared<network_t>(output_func, states);
    return network;
  }
};

} // namespace reelay
/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "any"
#include "map"
#include "memory"
#include "string"


#include "antlr4-runtime.h"

#include "reelay/parser/antlr4/temporal_logic/TemporalLogicBaseVisitor.h"
#include "reelay/parser/antlr4/temporal_logic/TemporalLogicLexer.h"
#include "reelay/parser/antlr4/temporal_logic/TemporalLogicParser.h"


#include "reelay/lexical_cast.hpp"
#include "reelay/networks.hpp"
#include "reelay/settings.hpp"

namespace reelay {

template <class Setting>
struct ReelayTemporalLogicVisitor : public TemporalLogicBaseVisitor {
  using input_t = typename Setting::input_t;
  using output_t = typename Setting::output_t;
  using function_t = typename Setting::function_t;

  using node_t = typename Setting::node_t;
  using state_t = typename Setting::state_t;
  using network_t = typename Setting::network_t;

  std::map<std::string, function_t> atoms;

  std::vector<std::shared_ptr<state_t>> states;

  ReelayTemporalLogicVisitor(std::map<std::string, function_t> m) : atoms(m) {}

  antlrcpp::Any visitProposition(
      TemporalLogicParser::PropositionContext* ctx) override {
    auto name = ctx->name->getText();
    auto expr = Setting::make_proposition(name);

    states.push_back(expr);

    return std::static_pointer_cast<node_t>(expr);
  }

  antlrcpp::Any visitBasicPredicateGT(
      TemporalLogicParser::BasicPredicateGTContext* ctx) override {
    // std::size_t i = boost::lexical_cast<std::size_t>(ctx->index->getText());
    auto name = ctx->name->getText();
    float c = boost::lexical_cast<float>(ctx->constant->getText());

    auto expr = Setting::make_basic_predicate(name, "gt", c);

    states.push_back(expr);

    return std::static_pointer_cast<node_t>(expr);
  }

  antlrcpp::Any visitBasicPredicateGE(
      TemporalLogicParser::BasicPredicateGEContext* ctx) override {
    auto name = ctx->name->getText();
    float c = boost::lexical_cast<float>(ctx->constant->getText());

    auto expr = Setting::make_basic_predicate(name, "ge", c);

    states.push_back(expr);

    return std::static_pointer_cast<node_t>(expr);
  }

  antlrcpp::Any visitBasicPredicateLT(
      TemporalLogicParser::BasicPredicateLTContext* ctx) override {
    auto name = ctx->name->getText();
    float c = boost::lexical_cast<float>(ctx->constant->getText());

    auto expr = Setting::make_basic_predicate(name, "lt", c);

    states.push_back(expr);

    return std::static_pointer_cast<node_t>(expr);
  }

  antlrcpp::Any visitBasicPredicateLE(
      TemporalLogicParser::BasicPredicateLEContext* ctx) override {
    auto name = ctx->name->getText();
    float c = boost::lexical_cast<float>(ctx->constant->getText());

    auto expr = Setting::make_basic_predicate(name, "le", c);

    states.push_back(expr);

    return std::static_pointer_cast<node_t>(expr);
  }

  antlrcpp::Any visitNamedPredicate(
      TemporalLogicParser::NamedPredicateContext* ctx) override {
    auto name = ctx->name->getText();
    auto func = atoms[name];

    auto expr = Setting::make_predicate(func);

    states.push_back(expr);

    return std::static_pointer_cast<node_t>(expr);
  }

  antlrcpp::Any visitMain(TemporalLogicParser::MainContext* ctx) override {
    return this->visit(ctx->child);
  }

  antlrcpp::Any visitGrouping(
      TemporalLogicParser::GroupingContext* ctx) override {
    return this->visit(ctx->child);
  }

  antlrcpp::Any visitAtomic(TemporalLogicParser::AtomicContext* ctx) override {
    return this->visit(ctx->child);
  }

  antlrcpp::Any visitNegation(
      TemporalLogicParser::NegationContext* ctx) override {
    auto child = this->visit(ctx->child);

    auto args = std::vector<std::shared_ptr<node_t>>({child});
    auto expr = Setting::make_node("negation", args);

    return std::static_pointer_cast<node_t>(expr);
  }

  antlrcpp::Any visitDisjunction(
      TemporalLogicParser::DisjunctionContext* ctx) override {
    auto left = this->visit(ctx->left);
    auto right = this->visit(ctx->right);

    auto args = std::vector<std::shared_ptr<node_t>>({left, right});
    auto expr = Setting::make_node("disjunction", args);

    return std::static_pointer_cast<node_t>(expr);
  }

  antlrcpp::Any visitConjunction(
      TemporalLogicParser::ConjunctionContext* ctx) override {
    auto left = this->visit(ctx->left);
    auto right = this->visit(ctx->right);

    auto args = std::vector<std::shared_ptr<node_t>>({left, right});
    auto expr = Setting::make_node("conjunction", args);

    return std::static_pointer_cast<node_t>(expr);
  }

  antlrcpp::Any visitImplication(
      TemporalLogicParser::ImplicationContext* ctx) override {
    auto left = this->visit(ctx->left);
    auto right = this->visit(ctx->right);

    auto args = std::vector<std::shared_ptr<node_t>>({left, right});
    auto expr = Setting::make_node("implication", args);

    return std::static_pointer_cast<node_t>(expr);
  }

  antlrcpp::Any visitSometimeP(
      TemporalLogicParser::SometimePContext* ctx) override {
    auto child = this->visit(ctx->child);

    auto args = std::vector<std::shared_ptr<node_t>>({child});
    auto expr = Setting::make_state("past_sometime", args);

    states.push_back(expr);

    return std::static_pointer_cast<node_t>(expr);
  }

  antlrcpp::Any visitAlwaysP(
      TemporalLogicParser::AlwaysPContext* ctx) override {
    auto child = this->visit(ctx->child);

    auto args = std::vector<std::shared_ptr<node_t>>({child});
    auto expr = Setting::make_state("past_always", args);

    states.push_back(expr);

    return std::static_pointer_cast<node_t>(expr);
  }

  antlrcpp::Any visitSince(TemporalLogicParser::SinceContext* ctx) override {
    auto left = this->visit(ctx->left);
    auto right = this->visit(ctx->right);

    auto args = std::vector<std::shared_ptr<node_t>>({left, right});
    auto expr = Setting::make_state("since", args);

    states.push_back(expr);

    return std::static_pointer_cast<node_t>(expr);
  }

  antlrcpp::Any visitTimedSometimeP(
      TemporalLogicParser::TimedSometimePContext* ctx) override {
    auto child = this->visit(ctx->child);

    std::pair<float, float> bounds = this->visit(ctx->bound);

    auto args = std::vector<std::shared_ptr<node_t>>({child});
    auto expr = Setting::make_state("past_sometime_bounded", args, bounds);
    ;

    states.push_back(expr);

    return std::static_pointer_cast<node_t>(expr);
  }

  antlrcpp::Any visitTimedAlwaysP(
      TemporalLogicParser::TimedAlwaysPContext* ctx) override {
    auto child = this->visit(ctx->child);

    std::pair<float, float> bounds = this->visit(ctx->bound);

    auto args = std::vector<std::shared_ptr<node_t>>({child});
    auto expr = Setting::make_state("past_always_bounded", args, bounds);
    ;

    states.push_back(expr);

    return std::static_pointer_cast<node_t>(expr);
  }

  antlrcpp::Any visitTimedSince(
      TemporalLogicParser::TimedSinceContext* ctx) override {
    auto left = this->visit(ctx->left);
    auto right = this->visit(ctx->right);

    std::pair<float, float> bounds = this->visit(ctx->bound);

    auto args = std::vector<std::shared_ptr<node_t>>({left, right});
    auto expr = Setting::make_state("since_bounded", args, bounds);
    ;

    states.push_back(expr);

    return std::static_pointer_cast<node_t>(expr);
  }

  antlrcpp::Any visitLowerBound(
      TemporalLogicParser::LowerBoundContext* ctx) override {
    float lbound = boost::lexical_cast<float>(ctx->l->getText());
    float ubound = 0;

    // std::cout << lbound << ubound << std::endl;

    return std::make_pair(lbound, ubound);
  }

  antlrcpp::Any visitUpperBound(
      TemporalLogicParser::UpperBoundContext* ctx) override {
    float lbound = 0;
    float ubound = boost::lexical_cast<float>(ctx->u->getText());

    // std::cout << lbound << ubound << std::endl;

    return std::make_pair(lbound, ubound);
  }

  antlrcpp::Any visitBothBound(
      TemporalLogicParser::BothBoundContext* ctx) override {
    float lbound = boost::lexical_cast<float>(ctx->l->getText());
    float ubound = boost::lexical_cast<float>(ctx->u->getText());

    // std::cout << lbound << ubound << std::endl;

    return std::make_pair(lbound, ubound);
  }
};

class MyParserErrorListener : public antlr4::BaseErrorListener {
  virtual void syntaxError(antlr4::Recognizer* recognizer,
                           antlr4::Token* offendingSymbol,
                           size_t line,
                           size_t charPositionInLine,
                           const std::string& msg,
                           std::exception_ptr e) override {
    // std::ostrstream s;
    // std::cerr << "Line(" << line << ":" << charPositionInLine << ") Error("
    // << msg << ")";
    throw std::invalid_argument(msg);
  }
};

template <class Setting>
struct make_network {
  using input_t = typename Setting::input_t;
  using output_t = typename Setting::output_t;
  using function_t = typename Setting::function_t;

  using network_t = typename Setting::network_t;
  using network_ptr_t = typename Setting::network_ptr_t;

  static network_ptr_t from_temporal_logic(
      std::string pattern,
      std::map<std::string, function_t> predicates) {
    std::ifstream stream;
    stream.open(pattern);

    antlr4::ANTLRInputStream input(pattern);
    TemporalLogicLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    TemporalLogicParser parser(&tokens);

    MyParserErrorListener errorListener;
    parser.removeErrorListeners();
    parser.addErrorListener(&errorListener);

    antlr4::tree::ParseTree* tree = parser.main();
    // std::cout << tree->toStringTree() << std::endl;

    auto visitor = ReelayTemporalLogicVisitor<Setting>(predicates);  // Wrong
    auto output_func = visitor.visit(tree);

    auto network = std::make_shared<network_t>(output_func, visitor.states);

    return network;
  }

  // static network_t from_regular_expressions(
  //    std::string pattern,
  //    std::map<std::string, std::function<base_t(const input_t &)>> atoms);
};

}  // namespace reelay
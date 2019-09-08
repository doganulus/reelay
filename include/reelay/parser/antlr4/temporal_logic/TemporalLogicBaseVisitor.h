
// Generated from grammar/antlr4/TemporalLogic.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"
#include "TemporalLogicVisitor.h"


/**
 * This class provides an empty implementation of TemporalLogicVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  TemporalLogicBaseVisitor : public TemporalLogicVisitor {
public:

  virtual antlrcpp::Any visitMain(TemporalLogicParser::MainContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTimedSometimeP(TemporalLogicParser::TimedSometimePContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTimedUntil(TemporalLogicParser::TimedUntilContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPre(TemporalLogicParser::PreContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSometimeF(TemporalLogicParser::SometimeFContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNegation(TemporalLogicParser::NegationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTimedSince(TemporalLogicParser::TimedSinceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTimedAlwaysP(TemporalLogicParser::TimedAlwaysPContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSometimeP(TemporalLogicParser::SometimePContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAtomic(TemporalLogicParser::AtomicContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGrouping(TemporalLogicParser::GroupingContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAlwaysF(TemporalLogicParser::AlwaysFContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTimedAlwaysF(TemporalLogicParser::TimedAlwaysFContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitImplication(TemporalLogicParser::ImplicationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDisjunction(TemporalLogicParser::DisjunctionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNext(TemporalLogicParser::NextContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTimedSometimeF(TemporalLogicParser::TimedSometimeFContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConjunction(TemporalLogicParser::ConjunctionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSince(TemporalLogicParser::SinceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAlwaysP(TemporalLogicParser::AlwaysPContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUntil(TemporalLogicParser::UntilContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitProposition(TemporalLogicParser::PropositionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBasicPredicateLT(TemporalLogicParser::BasicPredicateLTContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBasicPredicateLE(TemporalLogicParser::BasicPredicateLEContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBasicPredicateGT(TemporalLogicParser::BasicPredicateGTContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBasicPredicateGE(TemporalLogicParser::BasicPredicateGEContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNamedPredicate(TemporalLogicParser::NamedPredicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLowerBound(TemporalLogicParser::LowerBoundContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUpperBound(TemporalLogicParser::UpperBoundContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBothBound(TemporalLogicParser::BothBoundContext *ctx) override {
    return visitChildren(ctx);
  }


};


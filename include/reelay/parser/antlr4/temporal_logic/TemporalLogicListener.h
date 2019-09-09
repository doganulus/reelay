
// Generated from grammar/antlr4/TemporalLogic.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"
#include "TemporalLogicParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by TemporalLogicParser.
 */
class  TemporalLogicListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterMain(TemporalLogicParser::MainContext *ctx) = 0;
  virtual void exitMain(TemporalLogicParser::MainContext *ctx) = 0;

  virtual void enterTimedSometimeP(TemporalLogicParser::TimedSometimePContext *ctx) = 0;
  virtual void exitTimedSometimeP(TemporalLogicParser::TimedSometimePContext *ctx) = 0;

  virtual void enterTimedUntil(TemporalLogicParser::TimedUntilContext *ctx) = 0;
  virtual void exitTimedUntil(TemporalLogicParser::TimedUntilContext *ctx) = 0;

  virtual void enterPre(TemporalLogicParser::PreContext *ctx) = 0;
  virtual void exitPre(TemporalLogicParser::PreContext *ctx) = 0;

  virtual void enterSometimeF(TemporalLogicParser::SometimeFContext *ctx) = 0;
  virtual void exitSometimeF(TemporalLogicParser::SometimeFContext *ctx) = 0;

  virtual void enterNegation(TemporalLogicParser::NegationContext *ctx) = 0;
  virtual void exitNegation(TemporalLogicParser::NegationContext *ctx) = 0;

  virtual void enterTimedSince(TemporalLogicParser::TimedSinceContext *ctx) = 0;
  virtual void exitTimedSince(TemporalLogicParser::TimedSinceContext *ctx) = 0;

  virtual void enterTimedAlwaysP(TemporalLogicParser::TimedAlwaysPContext *ctx) = 0;
  virtual void exitTimedAlwaysP(TemporalLogicParser::TimedAlwaysPContext *ctx) = 0;

  virtual void enterSometimeP(TemporalLogicParser::SometimePContext *ctx) = 0;
  virtual void exitSometimeP(TemporalLogicParser::SometimePContext *ctx) = 0;

  virtual void enterAtomic(TemporalLogicParser::AtomicContext *ctx) = 0;
  virtual void exitAtomic(TemporalLogicParser::AtomicContext *ctx) = 0;

  virtual void enterGrouping(TemporalLogicParser::GroupingContext *ctx) = 0;
  virtual void exitGrouping(TemporalLogicParser::GroupingContext *ctx) = 0;

  virtual void enterAlwaysF(TemporalLogicParser::AlwaysFContext *ctx) = 0;
  virtual void exitAlwaysF(TemporalLogicParser::AlwaysFContext *ctx) = 0;

  virtual void enterTimedAlwaysF(TemporalLogicParser::TimedAlwaysFContext *ctx) = 0;
  virtual void exitTimedAlwaysF(TemporalLogicParser::TimedAlwaysFContext *ctx) = 0;

  virtual void enterImplication(TemporalLogicParser::ImplicationContext *ctx) = 0;
  virtual void exitImplication(TemporalLogicParser::ImplicationContext *ctx) = 0;

  virtual void enterDisjunction(TemporalLogicParser::DisjunctionContext *ctx) = 0;
  virtual void exitDisjunction(TemporalLogicParser::DisjunctionContext *ctx) = 0;

  virtual void enterNext(TemporalLogicParser::NextContext *ctx) = 0;
  virtual void exitNext(TemporalLogicParser::NextContext *ctx) = 0;

  virtual void enterTimedSometimeF(TemporalLogicParser::TimedSometimeFContext *ctx) = 0;
  virtual void exitTimedSometimeF(TemporalLogicParser::TimedSometimeFContext *ctx) = 0;

  virtual void enterConjunction(TemporalLogicParser::ConjunctionContext *ctx) = 0;
  virtual void exitConjunction(TemporalLogicParser::ConjunctionContext *ctx) = 0;

  virtual void enterSince(TemporalLogicParser::SinceContext *ctx) = 0;
  virtual void exitSince(TemporalLogicParser::SinceContext *ctx) = 0;

  virtual void enterAlwaysP(TemporalLogicParser::AlwaysPContext *ctx) = 0;
  virtual void exitAlwaysP(TemporalLogicParser::AlwaysPContext *ctx) = 0;

  virtual void enterUntil(TemporalLogicParser::UntilContext *ctx) = 0;
  virtual void exitUntil(TemporalLogicParser::UntilContext *ctx) = 0;

  virtual void enterProposition(TemporalLogicParser::PropositionContext *ctx) = 0;
  virtual void exitProposition(TemporalLogicParser::PropositionContext *ctx) = 0;

  virtual void enterBasicPredicateLT(TemporalLogicParser::BasicPredicateLTContext *ctx) = 0;
  virtual void exitBasicPredicateLT(TemporalLogicParser::BasicPredicateLTContext *ctx) = 0;

  virtual void enterBasicPredicateLE(TemporalLogicParser::BasicPredicateLEContext *ctx) = 0;
  virtual void exitBasicPredicateLE(TemporalLogicParser::BasicPredicateLEContext *ctx) = 0;

  virtual void enterBasicPredicateGT(TemporalLogicParser::BasicPredicateGTContext *ctx) = 0;
  virtual void exitBasicPredicateGT(TemporalLogicParser::BasicPredicateGTContext *ctx) = 0;

  virtual void enterBasicPredicateGE(TemporalLogicParser::BasicPredicateGEContext *ctx) = 0;
  virtual void exitBasicPredicateGE(TemporalLogicParser::BasicPredicateGEContext *ctx) = 0;

  virtual void enterNamedPredicate(TemporalLogicParser::NamedPredicateContext *ctx) = 0;
  virtual void exitNamedPredicate(TemporalLogicParser::NamedPredicateContext *ctx) = 0;

  virtual void enterLowerBound(TemporalLogicParser::LowerBoundContext *ctx) = 0;
  virtual void exitLowerBound(TemporalLogicParser::LowerBoundContext *ctx) = 0;

  virtual void enterUpperBound(TemporalLogicParser::UpperBoundContext *ctx) = 0;
  virtual void exitUpperBound(TemporalLogicParser::UpperBoundContext *ctx) = 0;

  virtual void enterBothBound(TemporalLogicParser::BothBoundContext *ctx) = 0;
  virtual void exitBothBound(TemporalLogicParser::BothBoundContext *ctx) = 0;


};


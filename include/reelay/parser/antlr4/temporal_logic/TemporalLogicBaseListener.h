
// Generated from grammar/antlr4/TemporalLogic.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"
#include "TemporalLogicListener.h"


/**
 * This class provides an empty implementation of TemporalLogicListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  TemporalLogicBaseListener : public TemporalLogicListener {
public:

  virtual void enterMain(TemporalLogicParser::MainContext * /*ctx*/) override { }
  virtual void exitMain(TemporalLogicParser::MainContext * /*ctx*/) override { }

  virtual void enterTimedSometimeP(TemporalLogicParser::TimedSometimePContext * /*ctx*/) override { }
  virtual void exitTimedSometimeP(TemporalLogicParser::TimedSometimePContext * /*ctx*/) override { }

  virtual void enterTimedUntil(TemporalLogicParser::TimedUntilContext * /*ctx*/) override { }
  virtual void exitTimedUntil(TemporalLogicParser::TimedUntilContext * /*ctx*/) override { }

  virtual void enterPre(TemporalLogicParser::PreContext * /*ctx*/) override { }
  virtual void exitPre(TemporalLogicParser::PreContext * /*ctx*/) override { }

  virtual void enterSometimeF(TemporalLogicParser::SometimeFContext * /*ctx*/) override { }
  virtual void exitSometimeF(TemporalLogicParser::SometimeFContext * /*ctx*/) override { }

  virtual void enterNegation(TemporalLogicParser::NegationContext * /*ctx*/) override { }
  virtual void exitNegation(TemporalLogicParser::NegationContext * /*ctx*/) override { }

  virtual void enterTimedSince(TemporalLogicParser::TimedSinceContext * /*ctx*/) override { }
  virtual void exitTimedSince(TemporalLogicParser::TimedSinceContext * /*ctx*/) override { }

  virtual void enterTimedAlwaysP(TemporalLogicParser::TimedAlwaysPContext * /*ctx*/) override { }
  virtual void exitTimedAlwaysP(TemporalLogicParser::TimedAlwaysPContext * /*ctx*/) override { }

  virtual void enterSometimeP(TemporalLogicParser::SometimePContext * /*ctx*/) override { }
  virtual void exitSometimeP(TemporalLogicParser::SometimePContext * /*ctx*/) override { }

  virtual void enterAtomic(TemporalLogicParser::AtomicContext * /*ctx*/) override { }
  virtual void exitAtomic(TemporalLogicParser::AtomicContext * /*ctx*/) override { }

  virtual void enterGrouping(TemporalLogicParser::GroupingContext * /*ctx*/) override { }
  virtual void exitGrouping(TemporalLogicParser::GroupingContext * /*ctx*/) override { }

  virtual void enterAlwaysF(TemporalLogicParser::AlwaysFContext * /*ctx*/) override { }
  virtual void exitAlwaysF(TemporalLogicParser::AlwaysFContext * /*ctx*/) override { }

  virtual void enterTimedAlwaysF(TemporalLogicParser::TimedAlwaysFContext * /*ctx*/) override { }
  virtual void exitTimedAlwaysF(TemporalLogicParser::TimedAlwaysFContext * /*ctx*/) override { }

  virtual void enterImplication(TemporalLogicParser::ImplicationContext * /*ctx*/) override { }
  virtual void exitImplication(TemporalLogicParser::ImplicationContext * /*ctx*/) override { }

  virtual void enterDisjunction(TemporalLogicParser::DisjunctionContext * /*ctx*/) override { }
  virtual void exitDisjunction(TemporalLogicParser::DisjunctionContext * /*ctx*/) override { }

  virtual void enterNext(TemporalLogicParser::NextContext * /*ctx*/) override { }
  virtual void exitNext(TemporalLogicParser::NextContext * /*ctx*/) override { }

  virtual void enterTimedSometimeF(TemporalLogicParser::TimedSometimeFContext * /*ctx*/) override { }
  virtual void exitTimedSometimeF(TemporalLogicParser::TimedSometimeFContext * /*ctx*/) override { }

  virtual void enterConjunction(TemporalLogicParser::ConjunctionContext * /*ctx*/) override { }
  virtual void exitConjunction(TemporalLogicParser::ConjunctionContext * /*ctx*/) override { }

  virtual void enterSince(TemporalLogicParser::SinceContext * /*ctx*/) override { }
  virtual void exitSince(TemporalLogicParser::SinceContext * /*ctx*/) override { }

  virtual void enterAlwaysP(TemporalLogicParser::AlwaysPContext * /*ctx*/) override { }
  virtual void exitAlwaysP(TemporalLogicParser::AlwaysPContext * /*ctx*/) override { }

  virtual void enterUntil(TemporalLogicParser::UntilContext * /*ctx*/) override { }
  virtual void exitUntil(TemporalLogicParser::UntilContext * /*ctx*/) override { }

  virtual void enterProposition(TemporalLogicParser::PropositionContext * /*ctx*/) override { }
  virtual void exitProposition(TemporalLogicParser::PropositionContext * /*ctx*/) override { }

  virtual void enterBasicPredicateLT(TemporalLogicParser::BasicPredicateLTContext * /*ctx*/) override { }
  virtual void exitBasicPredicateLT(TemporalLogicParser::BasicPredicateLTContext * /*ctx*/) override { }

  virtual void enterBasicPredicateLE(TemporalLogicParser::BasicPredicateLEContext * /*ctx*/) override { }
  virtual void exitBasicPredicateLE(TemporalLogicParser::BasicPredicateLEContext * /*ctx*/) override { }

  virtual void enterBasicPredicateGT(TemporalLogicParser::BasicPredicateGTContext * /*ctx*/) override { }
  virtual void exitBasicPredicateGT(TemporalLogicParser::BasicPredicateGTContext * /*ctx*/) override { }

  virtual void enterBasicPredicateGE(TemporalLogicParser::BasicPredicateGEContext * /*ctx*/) override { }
  virtual void exitBasicPredicateGE(TemporalLogicParser::BasicPredicateGEContext * /*ctx*/) override { }

  virtual void enterNamedPredicate(TemporalLogicParser::NamedPredicateContext * /*ctx*/) override { }
  virtual void exitNamedPredicate(TemporalLogicParser::NamedPredicateContext * /*ctx*/) override { }

  virtual void enterLowerBound(TemporalLogicParser::LowerBoundContext * /*ctx*/) override { }
  virtual void exitLowerBound(TemporalLogicParser::LowerBoundContext * /*ctx*/) override { }

  virtual void enterUpperBound(TemporalLogicParser::UpperBoundContext * /*ctx*/) override { }
  virtual void exitUpperBound(TemporalLogicParser::UpperBoundContext * /*ctx*/) override { }

  virtual void enterBothBound(TemporalLogicParser::BothBoundContext * /*ctx*/) override { }
  virtual void exitBothBound(TemporalLogicParser::BothBoundContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};


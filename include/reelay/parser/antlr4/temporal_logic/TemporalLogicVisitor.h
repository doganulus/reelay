
// Generated from antlr4/grammar/TemporalLogic.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"
#include "TemporalLogicParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by TemporalLogicParser.
 */
class  TemporalLogicVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by TemporalLogicParser.
   */
    virtual antlrcpp::Any visitMain(TemporalLogicParser::MainContext *context) = 0;

    virtual antlrcpp::Any visitTimedSometimeP(TemporalLogicParser::TimedSometimePContext *context) = 0;

    virtual antlrcpp::Any visitTimedUntil(TemporalLogicParser::TimedUntilContext *context) = 0;

    virtual antlrcpp::Any visitPre(TemporalLogicParser::PreContext *context) = 0;

    virtual antlrcpp::Any visitSometimeF(TemporalLogicParser::SometimeFContext *context) = 0;

    virtual antlrcpp::Any visitNegation(TemporalLogicParser::NegationContext *context) = 0;

    virtual antlrcpp::Any visitTimedSince(TemporalLogicParser::TimedSinceContext *context) = 0;

    virtual antlrcpp::Any visitTimedAlwaysP(TemporalLogicParser::TimedAlwaysPContext *context) = 0;

    virtual antlrcpp::Any visitSometimeP(TemporalLogicParser::SometimePContext *context) = 0;

    virtual antlrcpp::Any visitAtomic(TemporalLogicParser::AtomicContext *context) = 0;

    virtual antlrcpp::Any visitGrouping(TemporalLogicParser::GroupingContext *context) = 0;

    virtual antlrcpp::Any visitAlwaysF(TemporalLogicParser::AlwaysFContext *context) = 0;

    virtual antlrcpp::Any visitTimedAlwaysF(TemporalLogicParser::TimedAlwaysFContext *context) = 0;

    virtual antlrcpp::Any visitImplication(TemporalLogicParser::ImplicationContext *context) = 0;

    virtual antlrcpp::Any visitDisjunction(TemporalLogicParser::DisjunctionContext *context) = 0;

    virtual antlrcpp::Any visitNext(TemporalLogicParser::NextContext *context) = 0;

    virtual antlrcpp::Any visitTimedSometimeF(TemporalLogicParser::TimedSometimeFContext *context) = 0;

    virtual antlrcpp::Any visitConjunction(TemporalLogicParser::ConjunctionContext *context) = 0;

    virtual antlrcpp::Any visitSince(TemporalLogicParser::SinceContext *context) = 0;

    virtual antlrcpp::Any visitAlwaysP(TemporalLogicParser::AlwaysPContext *context) = 0;

    virtual antlrcpp::Any visitUntil(TemporalLogicParser::UntilContext *context) = 0;

    virtual antlrcpp::Any visitProposition(TemporalLogicParser::PropositionContext *context) = 0;

    virtual antlrcpp::Any visitBasicPredicateLT(TemporalLogicParser::BasicPredicateLTContext *context) = 0;

    virtual antlrcpp::Any visitBasicPredicateLE(TemporalLogicParser::BasicPredicateLEContext *context) = 0;

    virtual antlrcpp::Any visitBasicPredicateGT(TemporalLogicParser::BasicPredicateGTContext *context) = 0;

    virtual antlrcpp::Any visitBasicPredicateGE(TemporalLogicParser::BasicPredicateGEContext *context) = 0;

    virtual antlrcpp::Any visitNamedPredicate(TemporalLogicParser::NamedPredicateContext *context) = 0;

    virtual antlrcpp::Any visitLowerBound(TemporalLogicParser::LowerBoundContext *context) = 0;

    virtual antlrcpp::Any visitUpperBound(TemporalLogicParser::UpperBoundContext *context) = 0;

    virtual antlrcpp::Any visitBothBound(TemporalLogicParser::BothBoundContext *context) = 0;


};


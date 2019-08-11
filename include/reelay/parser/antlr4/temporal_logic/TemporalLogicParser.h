
// Generated from antlr4/grammar/TemporalLogic.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"




class  TemporalLogicParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, NOT = 11, LOR = 12, LAND = 13, IMPLIES = 14, 
    PRE = 15, SOMETIME_P = 16, ALWAYS_P = 17, SINCE = 18, NEXT = 19, SOMETIME_F = 20, 
    ALWAYS_F = 21, UNTIL = 22, IDENTIFIER = 23, TIMESCALE = 24, NUMBER = 25, 
    DIGITS = 26, WS = 27
  };

  enum {
    RuleMain = 0, RuleExpr = 1, RuleAtom = 2, RuleTbound = 3
  };

  TemporalLogicParser(antlr4::TokenStream *input);
  ~TemporalLogicParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class MainContext;
  class ExprContext;
  class AtomContext;
  class TboundContext; 

  class  MainContext : public antlr4::ParserRuleContext {
  public:
    TemporalLogicParser::ExprContext *child = nullptr;;
    MainContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    ExprContext *expr();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MainContext* main();

  class  ExprContext : public antlr4::ParserRuleContext {
  public:
    ExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ExprContext() : antlr4::ParserRuleContext() { }
    void copyFrom(ExprContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  TimedSometimePContext : public ExprContext {
  public:
    TimedSometimePContext(ExprContext *ctx);

    TemporalLogicParser::TboundContext *bound = nullptr;
    TemporalLogicParser::ExprContext *child = nullptr;
    antlr4::tree::TerminalNode *SOMETIME_P();
    TboundContext *tbound();
    ExprContext *expr();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  TimedUntilContext : public ExprContext {
  public:
    TimedUntilContext(ExprContext *ctx);

    TemporalLogicParser::ExprContext *left = nullptr;
    TemporalLogicParser::TboundContext *bound = nullptr;
    TemporalLogicParser::ExprContext *right = nullptr;
    antlr4::tree::TerminalNode *UNTIL();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    TboundContext *tbound();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  PreContext : public ExprContext {
  public:
    PreContext(ExprContext *ctx);

    TemporalLogicParser::ExprContext *child = nullptr;
    antlr4::tree::TerminalNode *PRE();
    ExprContext *expr();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  SometimeFContext : public ExprContext {
  public:
    SometimeFContext(ExprContext *ctx);

    TemporalLogicParser::ExprContext *child = nullptr;
    antlr4::tree::TerminalNode *SOMETIME_F();
    ExprContext *expr();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  NegationContext : public ExprContext {
  public:
    NegationContext(ExprContext *ctx);

    TemporalLogicParser::ExprContext *child = nullptr;
    antlr4::tree::TerminalNode *NOT();
    ExprContext *expr();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  TimedSinceContext : public ExprContext {
  public:
    TimedSinceContext(ExprContext *ctx);

    TemporalLogicParser::ExprContext *left = nullptr;
    TemporalLogicParser::TboundContext *bound = nullptr;
    TemporalLogicParser::ExprContext *right = nullptr;
    antlr4::tree::TerminalNode *SINCE();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    TboundContext *tbound();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  TimedAlwaysPContext : public ExprContext {
  public:
    TimedAlwaysPContext(ExprContext *ctx);

    TemporalLogicParser::TboundContext *bound = nullptr;
    TemporalLogicParser::ExprContext *child = nullptr;
    antlr4::tree::TerminalNode *ALWAYS_P();
    TboundContext *tbound();
    ExprContext *expr();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  SometimePContext : public ExprContext {
  public:
    SometimePContext(ExprContext *ctx);

    TemporalLogicParser::ExprContext *child = nullptr;
    antlr4::tree::TerminalNode *SOMETIME_P();
    ExprContext *expr();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AtomicContext : public ExprContext {
  public:
    AtomicContext(ExprContext *ctx);

    TemporalLogicParser::AtomContext *child = nullptr;
    AtomContext *atom();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GroupingContext : public ExprContext {
  public:
    GroupingContext(ExprContext *ctx);

    TemporalLogicParser::ExprContext *child = nullptr;
    ExprContext *expr();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AlwaysFContext : public ExprContext {
  public:
    AlwaysFContext(ExprContext *ctx);

    TemporalLogicParser::ExprContext *child = nullptr;
    antlr4::tree::TerminalNode *ALWAYS_F();
    ExprContext *expr();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  TimedAlwaysFContext : public ExprContext {
  public:
    TimedAlwaysFContext(ExprContext *ctx);

    TemporalLogicParser::TboundContext *bound = nullptr;
    TemporalLogicParser::ExprContext *child = nullptr;
    antlr4::tree::TerminalNode *ALWAYS_F();
    TboundContext *tbound();
    ExprContext *expr();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ImplicationContext : public ExprContext {
  public:
    ImplicationContext(ExprContext *ctx);

    TemporalLogicParser::ExprContext *left = nullptr;
    TemporalLogicParser::ExprContext *right = nullptr;
    antlr4::tree::TerminalNode *IMPLIES();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  DisjunctionContext : public ExprContext {
  public:
    DisjunctionContext(ExprContext *ctx);

    TemporalLogicParser::ExprContext *left = nullptr;
    TemporalLogicParser::ExprContext *right = nullptr;
    antlr4::tree::TerminalNode *LOR();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  NextContext : public ExprContext {
  public:
    NextContext(ExprContext *ctx);

    TemporalLogicParser::ExprContext *child = nullptr;
    antlr4::tree::TerminalNode *NEXT();
    ExprContext *expr();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  TimedSometimeFContext : public ExprContext {
  public:
    TimedSometimeFContext(ExprContext *ctx);

    TemporalLogicParser::TboundContext *bound = nullptr;
    TemporalLogicParser::ExprContext *child = nullptr;
    antlr4::tree::TerminalNode *SOMETIME_F();
    TboundContext *tbound();
    ExprContext *expr();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ConjunctionContext : public ExprContext {
  public:
    ConjunctionContext(ExprContext *ctx);

    TemporalLogicParser::ExprContext *left = nullptr;
    TemporalLogicParser::ExprContext *right = nullptr;
    antlr4::tree::TerminalNode *LAND();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  SinceContext : public ExprContext {
  public:
    SinceContext(ExprContext *ctx);

    TemporalLogicParser::ExprContext *left = nullptr;
    TemporalLogicParser::ExprContext *right = nullptr;
    antlr4::tree::TerminalNode *SINCE();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AlwaysPContext : public ExprContext {
  public:
    AlwaysPContext(ExprContext *ctx);

    TemporalLogicParser::ExprContext *child = nullptr;
    antlr4::tree::TerminalNode *ALWAYS_P();
    ExprContext *expr();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  UntilContext : public ExprContext {
  public:
    UntilContext(ExprContext *ctx);

    TemporalLogicParser::ExprContext *left = nullptr;
    TemporalLogicParser::ExprContext *right = nullptr;
    antlr4::tree::TerminalNode *UNTIL();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ExprContext* expr();
  ExprContext* expr(int precedence);
  class  AtomContext : public antlr4::ParserRuleContext {
  public:
    AtomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    AtomContext() : antlr4::ParserRuleContext() { }
    void copyFrom(AtomContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  NamedPredicateContext : public AtomContext {
  public:
    NamedPredicateContext(AtomContext *ctx);

    antlr4::Token *name = nullptr;
    antlr4::tree::TerminalNode *IDENTIFIER();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  BasicPredicateGEContext : public AtomContext {
  public:
    BasicPredicateGEContext(AtomContext *ctx);

    antlr4::Token *name = nullptr;
    antlr4::Token *constant = nullptr;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *NUMBER();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  BasicPredicateGTContext : public AtomContext {
  public:
    BasicPredicateGTContext(AtomContext *ctx);

    antlr4::Token *name = nullptr;
    antlr4::Token *constant = nullptr;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *NUMBER();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  PropositionContext : public AtomContext {
  public:
    PropositionContext(AtomContext *ctx);

    antlr4::Token *name = nullptr;
    antlr4::tree::TerminalNode *IDENTIFIER();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  BasicPredicateLTContext : public AtomContext {
  public:
    BasicPredicateLTContext(AtomContext *ctx);

    antlr4::Token *name = nullptr;
    antlr4::Token *constant = nullptr;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *NUMBER();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  BasicPredicateLEContext : public AtomContext {
  public:
    BasicPredicateLEContext(AtomContext *ctx);

    antlr4::Token *name = nullptr;
    antlr4::Token *constant = nullptr;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *NUMBER();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  AtomContext* atom();

  class  TboundContext : public antlr4::ParserRuleContext {
  public:
    TboundContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    TboundContext() : antlr4::ParserRuleContext() { }
    void copyFrom(TboundContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  LowerBoundContext : public TboundContext {
  public:
    LowerBoundContext(TboundContext *ctx);

    antlr4::Token *l = nullptr;
    antlr4::tree::TerminalNode *NUMBER();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  BothBoundContext : public TboundContext {
  public:
    BothBoundContext(TboundContext *ctx);

    antlr4::Token *l = nullptr;
    antlr4::Token *u = nullptr;
    std::vector<antlr4::tree::TerminalNode *> NUMBER();
    antlr4::tree::TerminalNode* NUMBER(size_t i);
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  UpperBoundContext : public TboundContext {
  public:
    UpperBoundContext(TboundContext *ctx);

    antlr4::Token *u = nullptr;
    antlr4::tree::TerminalNode *NUMBER();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  TboundContext* tbound();


  virtual bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;
  bool exprSempred(ExprContext *_localctx, size_t predicateIndex);

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};


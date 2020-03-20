/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
 
#pragma once

namespace reelay {

struct ptl_grammar {

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
    Atom <- CustomPredicate / ListProposition / RecordProposition 
            
    CustomPredicate <- '$' LCURLY Name RCURLY

    ListProposition <- LBRACE ListingValue (COMMA ListingValue)* RBRACE
    RecordProposition <- LCURLY KeyValuePair (COMMA KeyValuePair)* RCURLY

    ListingValue <- ListingTrue / ListingFalse / ListingNumber / ListingString / ListingReference / ListingAnyValue / ListingEQ / ListingNE / ListingLT / ListingLE / ListingGT / ListingGE

    KeyValuePair <- KeyValuePairTrue / KeyValuePairFalse / KeyValuePairNumber / KeyValuePairString / KeyValuePairEQ / KeyValuePairNE / KeyValuePairLT / KeyValuePairLE / KeyValuePairGT / KeyValuePairGE / KeyValuePairReference / KeyValuePairAnyValue / KeyValueProp 

    ListingTrue <- TRUE
    ListingFalse <- FALSE
    ListingNumber <- Number
    ListingString <- String
    ListingAnyValue <- STAR 
    ListingReference <- STAR Name
    ListingEQ <- STAR EQ Number
    ListingNE <- STAR NE Number
    ListingLT <- STAR LT Number
    ListingLE <- STAR LE Number
    ListingGT <- STAR GT Number
    ListingGE <- STAR GE Number

    FieldKey <- Name (DOT Name)*
    KeyValueProp <- FieldKey
    KeyValuePairTrue <- FieldKey ':' TRUE
    KeyValuePairFalse <- FieldKey ':' FALSE
    KeyValuePairNumber <- FieldKey ':' Number
    KeyValuePairString <- FieldKey ':' String
    KeyValuePairAnyValue <- FieldKey ':' STAR
    KeyValuePairReference <- FieldKey ':' STAR Name
    KeyValuePairEQ <- FieldKey EQ Number
    KeyValuePairNE <- FieldKey NE Number
    KeyValuePairLT <- FieldKey LT Number
    KeyValuePairLE <- FieldKey LE Number
    KeyValuePairGT <- FieldKey GT Number
    KeyValuePairGE <- FieldKey GE Number

    NonEmptyVarList <- Name (COMMA Name)*
  
    Bound <- FullBound / LowerBound / UpperBound
    FullBound <-  "[" Number ":" Number "]"
    LowerBound <- "[" Number ":" 'inf'? "]"
    UpperBound <- "["        ":" Number "]"

    String <-  SQString / DQString / Name
    SQString <- SQ <[^']*> SQ
    DQString <- DQ <[^"]*> DQ
    Name   <- <[_a-zA-Z][_a-zA-Z0-9]*>
    Number <- <'-'? [0-9]+ ('.' [0-9]+)?>

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

    ~TRUE <- < 'true' >
    ~FALSE <- < 'false' >
            
    %whitespace <- [  \t\r\n]*
    )";
};

} // namespace reelay
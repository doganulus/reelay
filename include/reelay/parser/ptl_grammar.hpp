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
    Atom <- CustomPredicate / RecordProposition 
            
    CustomPredicate <- '$' LCURLY Name RCURLY
    
    RecordProposition <- NestedAnyRecordProposition / NestedAllRecordProposition / NestedRecordProposition / SimpleRecordProposition
    NestedAnyRecordProposition <- PathKey 'any' LCURLY KeyValuePair (COMMA KeyValuePair)* RCURLY
    NestedAllRecordProposition <- PathKey 'all' LCURLY KeyValuePair (COMMA KeyValuePair)* RCURLY
    NestedRecordProposition <- PathKey LCURLY KeyValuePair (COMMA KeyValuePair)* RCURLY
    SimpleRecordProposition <- LCURLY KeyValuePair (COMMA KeyValuePair)* RCURLY
 
    KeyValuePair <- RecordProposition/ KeyValuePairTrue / KeyValuePairFalse / KeyValuePairNumber / KeyValuePairString / KeyValuePairEQ / KeyValuePairNE / KeyValuePairLT / KeyValuePairLE / KeyValuePairGT / KeyValuePairGE / KeyValuePairReference / KeyValuePairAnyValue / KeyValueProp /ListingTrue / ListingFalse / ListingNumber / ListingString / ListingReference / ListingAnyValue / ListingEQ / ListingNE / ListingLT / ListingLE / ListingGT / ListingGE

    ArrayKey <- DOLLAR <[0-9]+>
    FieldKey <- String
    PathKey <- (String COLCOL)+

    ListingTrue <- ArrayKey ':' TRUE
    ListingFalse <- ArrayKey ':' FALSE
    ListingNumber <- ArrayKey ':' Number
    ListingString <- ArrayKey ':' String
    ListingAnyValue <- ArrayKey ':' STAR 
    ListingReference <- ArrayKey ':' STAR Name
    ListingEQ <- ArrayKey EQ Number
    ListingNE <- ArrayKey NE Number
    ListingLT <- ArrayKey LT Number
    ListingLE <- ArrayKey LE Number
    ListingGT <- ArrayKey GT Number
    ListingGE <- ArrayKey GE Number

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
    ~COLCOL <- < '::' >
    ~SQ <- < "'" >
    ~DQ <- < '"' >

    ~TRUE <- < 'true' >
    ~FALSE <- < 'false' >
            
    %whitespace <- [  \t\r\n]*
    )";
};

} // namespace reelay
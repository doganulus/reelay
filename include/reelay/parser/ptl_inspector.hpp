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
#include "reelay/third_party/cpp-peglib/peglib.h"

#include "reelay/common.hpp"
#include "reelay/parser/ptl_grammar.hpp"

namespace reelay {

struct ptl_inspector : ptl_grammar {

  peg::parser parser;
  reelay::kwargs meta =
      reelay::kwargs({{"timed", false}, {"has_references", false}});

  explicit ptl_inspector() {

    parser = peg::parser(grammar);
    parser.log = [](size_t line, size_t col, const std::string &msg) {
      std::cerr << line << ":" << col << ": " << msg << std::endl;
    };

    parser["KeyValuePairReference"] = [&](const peg::SemanticValues &sv) {
      this->meta["has_references"] = true;
    };

    parser["ExistsExpr"] = [&](const peg::SemanticValues &sv) {
      this->meta["has_references"] = true;
    };

    parser["ForallExpr"] = [&](const peg::SemanticValues &sv) {
      this->meta["has_references"] = true;
    };

    parser["TimedOnceExpr"] = [&](const peg::SemanticValues &sv) {
      this->meta["timed"] = true;
    };

    parser["TimedHistExpr"] = [&](const peg::SemanticValues &sv) {
      this->meta["timed"] = true;
    };

    parser["SinceExpr"] = [&](const peg::SemanticValues &sv) {
      if (sv.size() == 3) {
        this->meta["timed"] = true;
      }
    };

    parser.enable_packrat_parsing(); // Enable packrat parsing.
  }

  reelay::kwargs inspect(const std::string &pattern) {
    parser.parse(pattern.c_str());
    return this->meta;
  }
};

} // namespace reelay
#pragma once

#include "AST/AST.h"
#include "Parser.hh"
#include <FlexLexer.h>
#include <iostream>
#include <memory>
#include <string>

class Driver {
  public:
    using token_type = yy::parser::token_type;
    using semantic_type = yy::parser::semantic_type;

    Driver() {}

    std::unique_ptr<AST::ASTContext> parse();
    token_type yylex(semantic_type *yylval);

    AST::ASTContext result;
    yyFlexLexer lexer_;
};
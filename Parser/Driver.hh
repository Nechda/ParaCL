#pragma once 

#include <string>
#include <iostream>
#include "Parser.hh"
#include <FlexLexer.h>
#include "AST/AST.h"

class Driver {
  public:
    using token_type = yy::parser::token_type;
    using semantic_type = yy::parser::semantic_type;

    Driver() {}

    int parse();
    token_type yylex(semantic_type* yylval);


    AST::ASTContext result;
    yyFlexLexer lexer_;
};
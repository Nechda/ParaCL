#include "Driver.hh"
#include "Parser.hh"


int Driver::parse()
{
    yy::parser parser(*this);
    int ret_code = parser.parse();
    return ret_code;
}

Driver::token_type Driver::yylex(Driver::semantic_type* yylval) {
    auto tt = static_cast<token_type>(lexer_.yylex());
    switch (tt) {
        case token_type::TOK_NAME: yylval->build<std::string>() = lexer_.YYText(); break;
        case token_type::TOK_NUM:  yylval->build<std::string>() = lexer_.YYText(); break;
        default:break;
    }
    return tt;
}
#include "Driver.hh"
#include "Parser.hh"

std::unique_ptr<AST::ASTContext> Driver::parse() {
    yy::parser parser(*this);
    int ret_code = parser.parse();
    if (ret_code != 0)
        throw std::runtime_error("Parser return non zero code");

    auto ret_ctx = std::make_unique<AST::ASTContext>();
    std::swap(*ret_ctx, result);
    return std::move(ret_ctx);
}

Driver::token_type Driver::yylex(Driver::semantic_type *yylval) {
    auto tt = static_cast<token_type>(lexer_.yylex());
    switch (tt) {
    case token_type::TOK_NAME:
        yylval->build<std::string>() = lexer_.YYText();
        break;
    case token_type::TOK_NUM:
        yylval->build<std::string>() = lexer_.YYText();
        break;
    default:
        break;
    }
    return tt;
}
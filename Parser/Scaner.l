%option c++

%{

#include "Parser.hh"

%}

%option yylineno
%option noyywrap

%%
[ \n\t\v]      ;
[0-9]+         {return yy::parser::token::TOK_NUM;}
"+"            {return yy::parser::token::TOK_PLUS;}
"-"            {return yy::parser::token::TOK_MINUS;}
"*"            {return yy::parser::token::TOK_MUL;}
"/"            {return yy::parser::token::TOK_DIV;}

"=="           {return yy::parser::token::TOK_EQ;}
"!="           {return yy::parser::token::TOK_NE;}
">="           {return yy::parser::token::TOK_GE;}
"<="           {return yy::parser::token::TOK_LE;}
">"            {return yy::parser::token::TOK_GT;}
"<"            {return yy::parser::token::TOK_LT;}

"="            {return yy::parser::token::TOK_ASSIGN;}
";"            {return yy::parser::token::TOK_SEMI;}
"if"           {return yy::parser::token::TOK_IF;}
"("            {return yy::parser::token::TOK_L_PAR;}
")"            {return yy::parser::token::TOK_R_PAR;}
"{"            {return yy::parser::token::TOK_L_BRA;}
"}"            {return yy::parser::token::TOK_R_BRA;}
[a-zA-Z]+      {return yy::parser::token::TOK_NAME;}
.              {printf("invalid token ....\n"); yyterminate();}
%%

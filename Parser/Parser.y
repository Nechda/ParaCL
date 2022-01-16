%skeleton "lalr1.cc"

%define api.value.type variant

%code requires {
    #include <string>
    #include <iostream>
    #include <vector>

    #include "AST/AST.h"
    #include "ScopePtr.h"

    using safe_ptr = scoped_ptr<AST::ExprBase>;
    class Driver;
}

%param { Driver& drv }

%code {
    #include "Parser/Driver.hh"

    yy::parser::token_type yylex(yy::parser::semantic_type* yylval, Driver& drv);

    AST::ExprBase* insert_helper(std::vector<AST::ExprBase*>& vec, AST::ExprBase* elem);
}

%define api.token.prefix {TOK_}

%token <std::string> NUM
%token <std::string> NAME

%token <int>  IF
%token <int>  WHILE

%token <int>  L_PAR
%token <int>  R_PAR
%token <int>  L_BRA
%token <int>  R_BRA
%token <int>  ASSIGN

%token <int>  EQ   // equal
%token <int>  NE   // not equal
%token <int>  GE   // greater or equal
%token <int>  LE   // less or equal
%token <int>  GT   // greater than
%token <int>  LT   // less than

%left <int>  PLUS
%left <int>  MINUS
%left <int>  MUL
%left <int>  DIV

%token <int>  SEMI

%nterm <AST::ExprBase*> expr_ret
%nterm <AST::ExprBase*> expr_void
%nterm <AST::ExprBase*> expr_gen
%nterm <AST::ExprBase*> line

%nterm <std::vector<AST::ExprBase*>> lines
%nterm <AST::Block> block
%nterm <std::vector<AST::Block>> input

%start input

%%

input: input block {}
     | block       {}
     |             {}
;
block: lines IF L_PAR expr_ret R_PAR L_BRA block R_BRA block
          {
               auto& cmds = $1;
               auto& cond_block = $7;
               auto& next_block = $9;
               insert_helper(drv.result.nodes, new AST::CondBranchInst($4, cond_block.ID, next_block.ID));
               insert_helper(cmds, drv.result.nodes.back());
               drv.result.blocks.emplace_back(AST::Block(cmds, AST::Block::BranchReason::IF));
               $$ = drv.result.blocks.back();
          }
     | lines WHILE L_PAR expr_ret R_PAR L_BRA block R_BRA block
          {
               auto& cmds = $1;
               auto& cond_block = $7;
               auto& next_block = $9;
               insert_helper(drv.result.nodes, new AST::CondBranchInst($4, cond_block.ID, next_block.ID));
               insert_helper(cmds, drv.result.nodes.back());
               drv.result.blocks.emplace_back(AST::Block(cmds, AST::Block::BranchReason::WHILE));
               $$ = drv.result.blocks.back();
          }
     | lines 
          {
               drv.result.blocks.emplace_back(AST::Block($1, AST::Block::BranchReason::NONE));
               $$ = drv.result.blocks.back();
          }
;
lines: lines line { safe_ptr tmp_($2); $1.emplace_back(tmp_.get()); $$ = $1; tmp_.drop(); }
     | line       { safe_ptr tmp_($1); $$.emplace_back(tmp_.get()); tmp_.drop(); }
     |            {}
;
line: expr_gen SEMI                    { $$ = $1; }
expr_gen: expr_void | expr_ret;
expr_ret:   NUM                        { $$ = insert_helper(drv.result.nodes, new AST::ConstLineral(atoi($1.c_str())));}
          | NAME                       { $$ = insert_helper(drv.result.nodes, new AST::Variable($1)); }
          | expr_ret ASSIGN expr_ret   { $$ = insert_helper(drv.result.nodes, new AST::AssignInst($1, $3)); }
          | expr_ret GT expr_ret       { $$ = insert_helper(drv.result.nodes, new AST::BinaryInst($1, $3, AST::BinaryInst::OP::GT)); }
          | expr_ret LT expr_ret       { $$ = insert_helper(drv.result.nodes, new AST::BinaryInst($1, $3, AST::BinaryInst::OP::LT)); }
          | expr_ret GE expr_ret       { $$ = insert_helper(drv.result.nodes, new AST::BinaryInst($1, $3, AST::BinaryInst::OP::GE)); }
          | expr_ret LE expr_ret       { $$ = insert_helper(drv.result.nodes, new AST::BinaryInst($1, $3, AST::BinaryInst::OP::LE)); }
          | expr_ret EQ expr_ret       { $$ = insert_helper(drv.result.nodes, new AST::BinaryInst($1, $3, AST::BinaryInst::OP::EQ)); }
          | expr_ret NE expr_ret       { $$ = insert_helper(drv.result.nodes, new AST::BinaryInst($1, $3, AST::BinaryInst::OP::NE)); }
          | expr_ret DIV expr_ret      { $$ = insert_helper(drv.result.nodes, new AST::BinaryInst($1, $3, AST::BinaryInst::OP::DIV)); }
          | expr_ret MUL expr_ret      { $$ = insert_helper(drv.result.nodes, new AST::BinaryInst($1, $3, AST::BinaryInst::OP::MUL)); }
          | expr_ret MINUS expr_ret    { $$ = insert_helper(drv.result.nodes, new AST::BinaryInst($1, $3, AST::BinaryInst::OP::SUB)); }
          | expr_ret PLUS expr_ret     { $$ = insert_helper(drv.result.nodes, new AST::BinaryInst($1, $3, AST::BinaryInst::OP::ADD)); }
          | L_PAR expr_ret R_PAR       { $$ = $2;}
;
expr_void: NAME L_PAR expr_ret R_PAR
          {
               safe_ptr tmp_(new AST::FunctionCall($1, $3));
               drv.result.functions.insert($1);
               drv.result.nodes.push_back(tmp_.get());
               $$ = tmp_.get();
               tmp_.drop();
          }
;
%%


yy::parser::token_type yylex(yy::parser::semantic_type* yylval, Driver& drv) {
    return drv.yylex(yylval);
}

void yy::parser::error(const std::string& msg) {
    //std::cout << "Error: " << msg << std::endl;
    throw std::runtime_error("Syntax error");
}

AST::ExprBase* insert_helper(std::vector<AST::ExprBase*>& vec, AST::ExprBase* elem)
{
     safe_ptr elem_ptr(elem);
     vec.push_back(elem);
     elem_ptr.drop();
     return elem;
}
// A Bison parser, made by GNU Bison 3.7.5.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "Parser.hh"


// Unqualified %code blocks.
#line 19 "Parser.y"

    #include "Parser/Driver.hh"

    yy::parser::token_type yylex(yy::parser::semantic_type* yylval, Driver& drv);

#line 52 "Parser.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 125 "Parser.cc"

  /// Build a parser object.
  parser::parser (Driver& drv_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      drv (drv_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | symbol kinds.  |
  `---------------*/

  // basic_symbol.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
  {
    switch (this->kind ())
    {
      case symbol_kind::S_block: // block
        value.copy< AST::Block > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_line: // line
      case symbol_kind::S_expr_gen: // expr_gen
      case symbol_kind::S_expr_ret: // expr_ret
      case symbol_kind::S_expr_void: // expr_void
        value.copy< AST::ExprBase* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IF: // IF
      case symbol_kind::S_L_PAR: // L_PAR
      case symbol_kind::S_R_PAR: // R_PAR
      case symbol_kind::S_L_BRA: // L_BRA
      case symbol_kind::S_R_BRA: // R_BRA
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_PLUS: // PLUS
      case symbol_kind::S_MINUS: // MINUS
      case symbol_kind::S_MUL: // MUL
      case symbol_kind::S_DIV: // DIV
      case symbol_kind::S_SEMI: // SEMI
        value.copy< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUM: // NUM
      case symbol_kind::S_NAME: // NAME
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_input: // input
        value.copy< std::vector<AST::Block> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_lines: // lines
        value.copy< std::vector<AST::ExprBase*> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }



  template <typename Base>
  parser::symbol_kind_type
  parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }

  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_block: // block
        value.move< AST::Block > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_line: // line
      case symbol_kind::S_expr_gen: // expr_gen
      case symbol_kind::S_expr_ret: // expr_ret
      case symbol_kind::S_expr_void: // expr_void
        value.move< AST::ExprBase* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_IF: // IF
      case symbol_kind::S_L_PAR: // L_PAR
      case symbol_kind::S_R_PAR: // R_PAR
      case symbol_kind::S_L_BRA: // L_BRA
      case symbol_kind::S_R_BRA: // R_BRA
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_PLUS: // PLUS
      case symbol_kind::S_MINUS: // MINUS
      case symbol_kind::S_MUL: // MUL
      case symbol_kind::S_DIV: // DIV
      case symbol_kind::S_SEMI: // SEMI
        value.move< int > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_NUM: // NUM
      case symbol_kind::S_NAME: // NAME
        value.move< std::string > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_input: // input
        value.move< std::vector<AST::Block> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_lines: // lines
        value.move< std::vector<AST::ExprBase*> > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

  }

  // by_kind.
  parser::by_kind::by_kind ()
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  parser::by_kind::by_kind (by_kind&& that)
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  parser::by_kind::by_kind (const by_kind& that)
    : kind_ (that.kind_)
  {}

  parser::by_kind::by_kind (token_kind_type t)
    : kind_ (yytranslate_ (t))
  {}

  void
  parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  parser::symbol_kind_type
  parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }

  parser::symbol_kind_type
  parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_block: // block
        value.YY_MOVE_OR_COPY< AST::Block > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_line: // line
      case symbol_kind::S_expr_gen: // expr_gen
      case symbol_kind::S_expr_ret: // expr_ret
      case symbol_kind::S_expr_void: // expr_void
        value.YY_MOVE_OR_COPY< AST::ExprBase* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IF: // IF
      case symbol_kind::S_L_PAR: // L_PAR
      case symbol_kind::S_R_PAR: // R_PAR
      case symbol_kind::S_L_BRA: // L_BRA
      case symbol_kind::S_R_BRA: // R_BRA
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_PLUS: // PLUS
      case symbol_kind::S_MINUS: // MINUS
      case symbol_kind::S_MUL: // MUL
      case symbol_kind::S_DIV: // DIV
      case symbol_kind::S_SEMI: // SEMI
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUM: // NUM
      case symbol_kind::S_NAME: // NAME
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_input: // input
        value.YY_MOVE_OR_COPY< std::vector<AST::Block> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_lines: // lines
        value.YY_MOVE_OR_COPY< std::vector<AST::ExprBase*> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.kind ())
    {
      case symbol_kind::S_block: // block
        value.move< AST::Block > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_line: // line
      case symbol_kind::S_expr_gen: // expr_gen
      case symbol_kind::S_expr_ret: // expr_ret
      case symbol_kind::S_expr_void: // expr_void
        value.move< AST::ExprBase* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IF: // IF
      case symbol_kind::S_L_PAR: // L_PAR
      case symbol_kind::S_R_PAR: // R_PAR
      case symbol_kind::S_L_BRA: // L_BRA
      case symbol_kind::S_R_BRA: // R_BRA
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_PLUS: // PLUS
      case symbol_kind::S_MINUS: // MINUS
      case symbol_kind::S_MUL: // MUL
      case symbol_kind::S_DIV: // DIV
      case symbol_kind::S_SEMI: // SEMI
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUM: // NUM
      case symbol_kind::S_NAME: // NAME
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_input: // input
        value.move< std::vector<AST::Block> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_lines: // lines
        value.move< std::vector<AST::ExprBase*> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_block: // block
        value.copy< AST::Block > (that.value);
        break;

      case symbol_kind::S_line: // line
      case symbol_kind::S_expr_gen: // expr_gen
      case symbol_kind::S_expr_ret: // expr_ret
      case symbol_kind::S_expr_void: // expr_void
        value.copy< AST::ExprBase* > (that.value);
        break;

      case symbol_kind::S_IF: // IF
      case symbol_kind::S_L_PAR: // L_PAR
      case symbol_kind::S_R_PAR: // R_PAR
      case symbol_kind::S_L_BRA: // L_BRA
      case symbol_kind::S_R_BRA: // R_BRA
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_PLUS: // PLUS
      case symbol_kind::S_MINUS: // MINUS
      case symbol_kind::S_MUL: // MUL
      case symbol_kind::S_DIV: // DIV
      case symbol_kind::S_SEMI: // SEMI
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_NUM: // NUM
      case symbol_kind::S_NAME: // NAME
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_input: // input
        value.copy< std::vector<AST::Block> > (that.value);
        break;

      case symbol_kind::S_lines: // lines
        value.copy< std::vector<AST::ExprBase*> > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_block: // block
        value.move< AST::Block > (that.value);
        break;

      case symbol_kind::S_line: // line
      case symbol_kind::S_expr_gen: // expr_gen
      case symbol_kind::S_expr_ret: // expr_ret
      case symbol_kind::S_expr_void: // expr_void
        value.move< AST::ExprBase* > (that.value);
        break;

      case symbol_kind::S_IF: // IF
      case symbol_kind::S_L_PAR: // L_PAR
      case symbol_kind::S_R_PAR: // R_PAR
      case symbol_kind::S_L_BRA: // L_BRA
      case symbol_kind::S_R_BRA: // R_BRA
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_PLUS: // PLUS
      case symbol_kind::S_MINUS: // MINUS
      case symbol_kind::S_MUL: // MUL
      case symbol_kind::S_DIV: // DIV
      case symbol_kind::S_SEMI: // SEMI
        value.move< int > (that.value);
        break;

      case symbol_kind::S_NUM: // NUM
      case symbol_kind::S_NAME: // NAME
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_input: // input
        value.move< std::vector<AST::Block> > (that.value);
        break;

      case symbol_kind::S_lines: // lines
        value.move< std::vector<AST::ExprBase*> > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " (";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, drv));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_block: // block
        yylhs.value.emplace< AST::Block > ();
        break;

      case symbol_kind::S_line: // line
      case symbol_kind::S_expr_gen: // expr_gen
      case symbol_kind::S_expr_ret: // expr_ret
      case symbol_kind::S_expr_void: // expr_void
        yylhs.value.emplace< AST::ExprBase* > ();
        break;

      case symbol_kind::S_IF: // IF
      case symbol_kind::S_L_PAR: // L_PAR
      case symbol_kind::S_R_PAR: // R_PAR
      case symbol_kind::S_L_BRA: // L_BRA
      case symbol_kind::S_R_BRA: // R_BRA
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_PLUS: // PLUS
      case symbol_kind::S_MINUS: // MINUS
      case symbol_kind::S_MUL: // MUL
      case symbol_kind::S_DIV: // DIV
      case symbol_kind::S_SEMI: // SEMI
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_NUM: // NUM
      case symbol_kind::S_NAME: // NAME
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_input: // input
        yylhs.value.emplace< std::vector<AST::Block> > ();
        break;

      case symbol_kind::S_lines: // lines
        yylhs.value.emplace< std::vector<AST::ExprBase*> > ();
        break;

      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // input: input block
#line 58 "Parser.y"
                   {}
#line 860 "Parser.cc"
    break;

  case 3: // input: block
#line 59 "Parser.y"
                   {}
#line 866 "Parser.cc"
    break;

  case 4: // input: %empty
#line 60 "Parser.y"
                   {}
#line 872 "Parser.cc"
    break;

  case 5: // block: lines IF L_PAR expr_ret R_PAR L_BRA block R_BRA block
#line 63 "Parser.y"
          {
               auto& cmds = yystack_[8].value.as < std::vector<AST::ExprBase*> > ();
               auto& cond_block = yystack_[2].value.as < AST::Block > ();
               auto& next_block = yystack_[0].value.as < AST::Block > ();
               cmds.push_back(new AST::CondBranchInst(yystack_[5].value.as < AST::ExprBase* > (), cond_block.ID, next_block.ID));
               yylhs.value.as < AST::Block > () = AST::Block(cmds);
               drv.result.blocks.push_back(yylhs.value.as < AST::Block > ());
          }
#line 885 "Parser.cc"
    break;

  case 6: // block: lines
#line 72 "Parser.y"
          {
               yylhs.value.as < AST::Block > () = AST::Block(yystack_[0].value.as < std::vector<AST::ExprBase*> > ());
               drv.result.blocks.push_back(yylhs.value.as < AST::Block > ());
          }
#line 894 "Parser.cc"
    break;

  case 7: // lines: lines line
#line 77 "Parser.y"
                  { yystack_[1].value.as < std::vector<AST::ExprBase*> > ().push_back(yystack_[0].value.as < AST::ExprBase* > ()); yylhs.value.as < std::vector<AST::ExprBase*> > () = yystack_[1].value.as < std::vector<AST::ExprBase*> > (); }
#line 900 "Parser.cc"
    break;

  case 8: // lines: line
#line 78 "Parser.y"
                  { yylhs.value.as < std::vector<AST::ExprBase*> > ().push_back(yystack_[0].value.as < AST::ExprBase* > ()); }
#line 906 "Parser.cc"
    break;

  case 9: // lines: %empty
#line 79 "Parser.y"
                  {}
#line 912 "Parser.cc"
    break;

  case 10: // line: expr_gen SEMI
#line 81 "Parser.y"
                                       { yylhs.value.as < AST::ExprBase* > () = yystack_[1].value.as < AST::ExprBase* > (); }
#line 918 "Parser.cc"
    break;

  case 11: // expr_gen: expr_void
#line 82 "Parser.y"
          { yylhs.value.as < AST::ExprBase* > () = yystack_[0].value.as < AST::ExprBase* > (); }
#line 924 "Parser.cc"
    break;

  case 12: // expr_gen: expr_ret
#line 82 "Parser.y"
                      { yylhs.value.as < AST::ExprBase* > () = yystack_[0].value.as < AST::ExprBase* > (); }
#line 930 "Parser.cc"
    break;

  case 13: // expr_ret: NUM
#line 83 "Parser.y"
                                       { yylhs.value.as < AST::ExprBase* > () = new AST::ConstLineral(atoi(yystack_[0].value.as < std::string > ().c_str()));}
#line 936 "Parser.cc"
    break;

  case 14: // expr_ret: NAME
#line 84 "Parser.y"
                                       { yylhs.value.as < AST::ExprBase* > () = new AST::Variable(yystack_[0].value.as < std::string > ()); drv.result.variables.insert(yystack_[0].value.as < std::string > ()); }
#line 942 "Parser.cc"
    break;

  case 15: // expr_ret: expr_ret ASSIGN expr_ret
#line 85 "Parser.y"
                                       { yylhs.value.as < AST::ExprBase* > () = new AST::AssignInst(yystack_[2].value.as < AST::ExprBase* > (), yystack_[0].value.as < AST::ExprBase* > ()); }
#line 948 "Parser.cc"
    break;

  case 16: // expr_ret: expr_ret DIV expr_ret
#line 86 "Parser.y"
                                       { yylhs.value.as < AST::ExprBase* > () = new AST::BinaryInst(yystack_[2].value.as < AST::ExprBase* > (), yystack_[0].value.as < AST::ExprBase* > (), AST::BinaryInst::OP::DIV); }
#line 954 "Parser.cc"
    break;

  case 17: // expr_ret: expr_ret MUL expr_ret
#line 87 "Parser.y"
                                       { yylhs.value.as < AST::ExprBase* > () = new AST::BinaryInst(yystack_[2].value.as < AST::ExprBase* > (), yystack_[0].value.as < AST::ExprBase* > (), AST::BinaryInst::OP::MUL); }
#line 960 "Parser.cc"
    break;

  case 18: // expr_ret: expr_ret MINUS expr_ret
#line 88 "Parser.y"
                                       { yylhs.value.as < AST::ExprBase* > () = new AST::BinaryInst(yystack_[2].value.as < AST::ExprBase* > (), yystack_[0].value.as < AST::ExprBase* > (), AST::BinaryInst::OP::SUB); }
#line 966 "Parser.cc"
    break;

  case 19: // expr_ret: expr_ret PLUS expr_ret
#line 89 "Parser.y"
                                       { yylhs.value.as < AST::ExprBase* > () = new AST::BinaryInst(yystack_[2].value.as < AST::ExprBase* > (), yystack_[0].value.as < AST::ExprBase* > (), AST::BinaryInst::OP::ADD); }
#line 972 "Parser.cc"
    break;

  case 20: // expr_ret: L_PAR expr_ret R_PAR
#line 90 "Parser.y"
                                       { yylhs.value.as < AST::ExprBase* > () = yystack_[1].value.as < AST::ExprBase* > ();}
#line 978 "Parser.cc"
    break;

  case 21: // expr_void: NAME L_PAR expr_ret R_PAR
#line 92 "Parser.y"
                                     {yylhs.value.as < AST::ExprBase* > () = new AST::FunctionCall(yystack_[3].value.as < std::string > (), yystack_[1].value.as < AST::ExprBase* > ()); drv.result.functions.insert(yystack_[3].value.as < std::string > ()); }
#line 984 "Parser.cc"
    break;


#line 988 "Parser.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        std::string msg = YY_("syntax error");
        error (YY_MOVE (msg));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

#if YYDEBUG || 0
  const char *
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0





  const signed char parser::yypact_ninf_ = -12;

  const signed char parser::yytable_ninf_ = -1;

  const signed char
  parser::yypact_[] =
  {
      56,   -12,    -3,    60,     2,   -12,    52,   -12,   -11,    30,
     -12,    60,   -12,     0,   -12,   -12,     9,   -12,   -12,    60,
      60,    60,    60,    60,    15,   -12,    60,    30,    35,    40,
      20,     6,   -12,    25,    38,    56,    14,    56,   -12
  };

  const signed char
  parser::yydefact_[] =
  {
       4,    13,    14,     0,     9,     3,     6,     8,     0,    12,
      11,     0,    14,     0,     1,     2,     0,     7,    10,     0,
       0,     0,     0,     0,     0,    20,     0,    15,    19,    18,
      17,    16,    21,     0,     0,     9,     0,     9,     5
  };

  const signed char
  parser::yypgoto_[] =
  {
     -12,   -12,    -4,   -12,    45,   -12,    -2,   -12
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,     4,     5,     6,     7,     8,     9,    10
  };

  const signed char
  parser::yytable_[] =
  {
      15,    13,    14,    11,    18,     1,     2,    25,     3,    24,
      19,    20,    21,    22,    23,    26,    19,    27,    28,    29,
      30,    31,    32,    37,    33,    19,    20,    21,    22,    23,
      19,    36,    34,    38,    23,    19,    20,    21,    22,    23,
      19,    20,    21,    22,    23,    19,    35,    21,    22,    23,
      19,    17,     0,    22,    23,     1,     2,    16,     3,     1,
       2,     0,     3,     1,    12,     0,     3
  };

  const signed char
  parser::yycheck_[] =
  {
       4,     3,     0,     6,    15,     3,     4,     7,     6,    11,
      10,    11,    12,    13,    14,     6,    10,    19,    20,    21,
      22,    23,     7,     9,    26,    10,    11,    12,    13,    14,
      10,    35,     7,    37,    14,    10,    11,    12,    13,    14,
      10,    11,    12,    13,    14,    10,     8,    12,    13,    14,
      10,     6,    -1,    13,    14,     3,     4,     5,     6,     3,
       4,    -1,     6,     3,     4,    -1,     6
  };

  const signed char
  parser::yystos_[] =
  {
       0,     3,     4,     6,    17,    18,    19,    20,    21,    22,
      23,     6,     4,    22,     0,    18,     5,    20,    15,    10,
      11,    12,    13,    14,    22,     7,     6,    22,    22,    22,
      22,    22,     7,    22,     7,     8,    18,     9,    18
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    16,    17,    17,    17,    18,    18,    19,    19,    19,
      20,    21,    21,    22,    22,    22,    22,    22,    22,    22,
      22,    23
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     1,     0,     9,     1,     2,     1,     0,
       2,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     4
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "NUM", "NAME", "IF",
  "L_PAR", "R_PAR", "L_BRA", "R_BRA", "ASSIGN", "PLUS", "MINUS", "MUL",
  "DIV", "SEMI", "$accept", "input", "block", "lines", "line", "expr_gen",
  "expr_ret", "expr_void", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const signed char
  parser::yyrline_[] =
  {
       0,    58,    58,    59,    60,    62,    71,    77,    78,    79,
      81,    82,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    92
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  parser::symbol_kind_type
  parser::yytranslate_ (int t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15
    };
    // Last valid token kind.
    const int code_max = 270;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return YY_CAST (symbol_kind_type, translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // yy
#line 1357 "Parser.cc"

#line 94 "Parser.y"



yy::parser::token_type yylex(yy::parser::semantic_type* yylval, Driver& drv) {
    return drv.yylex(yylval);
}

void yy::parser::error(const std::string& msg) {
    std::cout << "Error: " << msg << std::endl;
}

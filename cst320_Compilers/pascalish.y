%{
//**************************************
// pascal.y
//
// Parser definition file. bison uses this file to generate the parser.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include <iostream>
#include "cSymbolTable.h"
#include "lex.h"
#include "astnodes.h"

#define CHECK_ERROR() { if (g_semanticErrorHappened) \
    { g_semanticErrorHappened = false; } }
#define PROP_ERROR() { if (g_semanticErrorHappened) \
    { g_semanticErrorHappened = false; YYERROR; } }

%}

%locations

 /* union defines the type for lexical values */
%union{
    int             int_val;
    double          real_val;
    cAstNode*       ast_node;
    cProgramNode*   program_node;
    cBlockNode*     block_node;
    cStmtsNode*     stmts_node;
    cStmtNode*      stmt_node;
    cExprNode*      expr_node;
    cSymbol*        symbol;
    cDeclsNode*     decls_node;
    cDeclNode*      decl_node;
    cExprsNode*     expr_list_node;
    cIdListNode*    id_list_node;
    cVarDeclsNode*  var_decls_node;
    cVarExprNode*   var_expr_node;
    cOpNode*        op_node;
    cProcDeclNode*  proc_decl_node;
    cFuncDeclNode*  func_decl_node;
    cFuncExprNode*  func_expr_node;
    }

%{
    int yyerror(const char *msg);

    cAstNode *yyast_root;
    static bool g_semanticErrorHappened;
    extern cSymbolTable g_sybolTable;
%}

%start  program

%token PROGRAM PROCEDURE FUNCTION FORWARD
%token DECLARE OPEN CLOSE
%token WRITE
%token WHILE IF THEN ELSE REPEAT UNTIL
%token FOR DO TO DOWNTO
%token CONST TYPE VAR
%token RECORD ARRAY OF
%token NIL
%token JUNK_TOKEN

%token <symbol>    IDENTIFIER
%token <symbol>    TYPE_ID
%token <int_val>   INT_VAL
%token <real_val>  REAL_VAL
%token <int_val>   AND
%token <int_val>   OR
%token <int_val>   NOT          /*int_val*/
%token <int_val>   DIV
%token <int_val>   MOD
%token <int_val>   NOT_EQUAL
%token <int_val>   LE
%token <int_val>   GE
%token <int_val>   ASSIGN

%type <program_node> program
%type <symbol> header
%type <block_node> block
%type <symbol> type
%type <decls_node> decls
%type <decls_node> constdecls
%type <decls_node> constdecl
%type <expr_node> constant
%type <decls_node> typedecls
%type <decls_node> typedecl
%type <decls_node> singleType
/* %type <ast_node> rangeList */
/* %type <ast_node> range */
%type <var_decls_node> goodvar
%type <var_decls_node> onevar
/* %type <ast_node> recorddef */
%type <var_decls_node> vardecls
%type <var_decls_node> vardecl;
%type <decls_node> procdecls
%type <var_decls_node> paramSpec
%type <proc_decl_node> procdecl
%type <var_decls_node> parlist
%type <id_list_node> idlist
%type <func_expr_node> func_call
%type <func_decl_node> funcProto
%type <func_decl_node> funcHeader
%type <proc_decl_node> procHeader
%type <stmts_node> statements
%type <stmt_node> statement
%type <expr_node> expr
%type <expr_node> oneExpr
%type <expr_node> addit
%type <expr_node> term
%type <expr_node> fact
%type <var_expr_node> variable
%type <var_expr_node> varpart
%type <expr_list_node> exprList
/* %type <ast_node> recHeader */

%%

program: header block '.'
                                { $$ = new cProgramNode($1, $2);
                                  yyast_root = $$;
                                  if (yynerrs == 0) 
                                      YYACCEPT;
                                  else
                                      YYABORT;
                                  PROP_ERROR();
                                }
header: PROGRAM IDENTIFIER ';'
                                {  g_symbolTable.IncreaseScope();
                                   $$ = $2; 
                                   CHECK_ERROR();
                                }
declHeader: DECLARE
                                { g_symbolTable.IncreaseScope(); CHECK_ERROR(); }
block:  declHeader decls OPEN statements CLOSE
                                {
                                    $$ = new cBlockNode($2, $4);
                                    g_symbolTable.DecreaseScope();
                                    PROP_ERROR();
                                }
     | OPEN statements CLOSE
                                { 
                                    $$ = new cBlockNode(nullptr, $2);
                                   PROP_ERROR(); 
                                }

decls: constdecls typedecls vardecls procdecls
                                {
                                    $$ = new cDeclsNode();
                                    $$->AddDecls($1);
                                    $$->AddDecls($2);
                                    $$->AddDecls($3);
                                    $$->AddDecls($4);
                                    CHECK_ERROR();
                                }
constdecls: CONST constdecl ';'
                                { $$ = $2; CHECK_ERROR(); }
        | /*empty */
                                { $$ = nullptr; CHECK_ERROR(); }
constdecl: constdecl ';' IDENTIFIER '=' constant 
                                { 
                                    $$ = $1;
                                    $$->AddDecl(new cConstDeclNode($3, $5));
                                    CHECK_ERROR();
                                }
        |  IDENTIFIER '=' constant 
                                { 
                                    $$ = new cDeclsNode(new cConstDeclNode($1, $3));
                                    CHECK_ERROR();
                                }
        | constdecl error ';'
                                { $$ = nullptr; }
typedecls: TYPE typedecl
                                { $$ = $2; CHECK_ERROR(); }
        | /*empty */
                                { $$ = nullptr; }
typedecl: typedecl singleType
                                { 
                                    $$ = $1;
                                    $$->AddDecls($2);
                                    CHECK_ERROR();
                                }
        |  singleType
                                { $$ = $1; CHECK_ERROR(); }
        |  error ';'
                                { $$ = nullptr; }

singleType: IDENTIFIER '=' TYPE_ID INT_VAL ';'
                                { 
                                    $$ = new cDeclsNode(new cBaseTypeNode($1, $3, $4)); 
                                    CHECK_ERROR();
                                }
                                /*
singleType:  IDENTIFIER '=' ARRAY '[' rangeList ']' OF type ';'
                                { }

        |    IDENTIFIER '=' recHeader recorddef ';'
                                { }

rangeList: rangeList ',' range
                                { }
        |  range
                                { }
range: INT_VAL '.' '.' INT_VAL
                                { }
*/
vardecls: VAR vardecl
                                { $$ = $2; CHECK_ERROR(); }
        | /* empty */
                                { $$ = nullptr; }
vardecl: vardecl onevar
                                {
                                   $$ = $1;
                                   $$->AddAllId($2);
                                   CHECK_ERROR();
                                }
        | onevar
                                { $$ = $1; CHECK_ERROR(); }
onevar: goodvar ';'
                                { $$ = $1; PROP_ERROR(); }
        | error ';'
                                { $$ = nullptr; }
goodvar: idlist ':' type
                                { $$ = new cVarDeclsNode($1, $3); CHECK_ERROR(); }
procdecls: procdecls procdecl
                                {
                                    if($$ == nullptr)
                                    {
                                        $$ = new cDeclsNode($2);
                                    }
                                    else
                                    {
                                        $$ = $1;
                                        $$->AddDecl($2);
                                    }
                                    PROP_ERROR();
                                }
        | /* empty */
                                { $$ = nullptr; }

procdecl: procHeader paramSpec ';' block ';'
                                {
                                    $$ = $1;
                                    $$->AddParams($2);
                                    $$->AddBlock($4);
                                    g_symbolTable.DecreaseScope();
                                    PROP_ERROR();
                                }
        |  procHeader paramSpec ';' FORWARD ';'
                                {
                                   $$ = $1; 
                                   $$->AddParams($2);
                                   g_symbolTable.DecreaseScope();
                                   CHECK_ERROR();
                                }
        |  funcProto ';' block ';'
                                { 
                                    $1->AddFuncBlock($3);
                                    $$ = $1;
                                    g_symbolTable.DecreaseScope();
                                    PROP_ERROR();
                                }
        |  funcProto ';' FORWARD ';'
                                { 
                                    $1->AddBlock(nullptr);
                                    $$ = $1;
                                    g_symbolTable.DecreaseScope();
                                    CHECK_ERROR();
                                }
        |  error ';' block ';'
                                {
                                    $$ = nullptr;
                                    g_symbolTable.DecreaseScope();
                                }
        |  error ';' FORWARD ';'
                                {
                                   $$ = nullptr; 
                                   g_symbolTable.DecreaseScope();
                                }
procHeader: PROCEDURE IDENTIFIER 
                                {
                                    $$ = new cProcDeclNode();
                                    $$->AddName($2);
                                    g_symbolTable.IncreaseScope();
                                    CHECK_ERROR();
                                }
funcHeader: FUNCTION IDENTIFIER
                                { 
                                    $$ = new cFuncDeclNode();
                                    $$->AddFuncName($2);
                                    g_symbolTable.IncreaseScope();
                                    PROP_ERROR();
                                }
funcProto: funcHeader paramSpec ':' type
                                { 
                                    $$ = $1;
                                    $$->AddFuncType($4->GetDecl());
                                    $$->AddFuncParams($2);
                                    //$$->AddFuncType($4->GetDecl());
                                    PROP_ERROR();
                                }
paramSpec: '(' parlist ')'
                                { $$ = $2; CHECK_ERROR(); }
        | /* empty */
                                { $$ = nullptr; }

idlist: idlist ',' IDENTIFIER
                                {
                                    $$ = $1;
                                    $$->AddSym($3);
                                    CHECK_ERROR();
                                }
    |    IDENTIFIER
                                { $$ = new cIdListNode($1); CHECK_ERROR(); }

parlist: parlist ';' VAR idlist ':' type 
                                {
                                    $$ = $1;
                                    $$->AddDecls(new cVarDeclsNode($4, $6));
                                    CHECK_ERROR();
                                }
    |    parlist ';' idlist ':' type 
                                {
                                    $$ = $1;
                                    $$->AddDecls(new cVarDeclsNode($3, $5));
                                    CHECK_ERROR();
                                }
    |    VAR idlist ':' type
                                { $$ = new cVarDeclsNode($2, $4); CHECK_ERROR(); }
    |    idlist ':' type
                                { $$ = new cVarDeclsNode($1, $3); CHECK_ERROR(); }

type: TYPE_ID
                                { }
/*
recHeader: RECORD
                                { }
recorddef: vardecl CLOSE
                                { }
*/
constant: INT_VAL
                                { $$ = new cIntExprNode($1); CHECK_ERROR(); }
    |   '-' INT_VAL
                                { 
                                    $$ = new cUnaryExprNode( new cOpNode('-'), 
                                                            new cIntExprNode($2));
                                    CHECK_ERROR();
                                }
    |   REAL_VAL
                                { $$ = new cRealExprNode($1); CHECK_ERROR(); }
    |   '-' REAL_VAL
                                { 
                                    $$ = new cUnaryExprNode( new cOpNode('-'), 
                                                            new cRealExprNode($2));
                                    CHECK_ERROR();
                                }

statements: statements statement
                                { 
                                    $$ = $1;
                                    $$->AddStmt($2);
                                    PROP_ERROR();
                                }
    |   statement
                                { $$ = new cStmtsNode($1); PROP_ERROR(); }

statement: variable ASSIGN expr ';'
                                { $$ = new cAssignNode($1, $3); PROP_ERROR(); }
    |   IF expr THEN statement
                                { 
                                    $$ = new cIfNode($2, $4);
                                    PROP_ERROR();
                                }
    |   IF expr THEN statement ELSE statement
                                { $$ = new cIfNode($2, $4, $6); PROP_ERROR(); }
/*
    |   REPEAT statements UNTIL expr ';'
                                { }
*/
    |   WHILE expr DO statement
                                { $$ = new cWhileNode($2, $4); PROP_ERROR(); }
    |   FOR IDENTIFIER ASSIGN expr TO expr DO statement
                                {}
    |   FOR IDENTIFIER ASSIGN expr DOWNTO expr DO statement
                                {}
    |   IDENTIFIER '(' exprList ')' ';'
                                { $$ = new cProcCallNode($1, $3); CHECK_ERROR(); }
    |   IDENTIFIER ';'
                                { $$ = new cProcCallNode($1, nullptr); CHECK_ERROR(); }
    |   WRITE '(' exprList ')' ';'
                                { $$ = new cWriteNode($3); CHECK_ERROR(); }
    |   block
                                { $$ = $1; }
    |   NIL ';'
                                { $$ = new cNilNode(); CHECK_ERROR(); }
    |   error ';'
                                { $$ = nullptr; }

exprList: exprList ',' oneExpr
                                { 
                                    $$ = $1; 
                                    $$->AddExpr($3);
                                    CHECK_ERROR();
                                }
        | oneExpr
                                { $$ = new cExprsNode($1); CHECK_ERROR(); }
        | /* empty */
                                { $$ = nullptr; }
oneExpr: expr
                                { $$ = $1; CHECK_ERROR(); }

func_call:  IDENTIFIER '(' exprList ')'
                                { $$ = new cFuncExprNode($1, $3); PROP_ERROR(); }

variable: variable '.' varpart
                                { }
        | variable '[' exprList ']'
                                { }
        | varpart
                                { $$ = $1; }

varpart:  IDENTIFIER
                                { $$ = new cVarExprNode($1); CHECK_ERROR(); }

expr:       expr '=' addit
                                { 
                                    $$ = new cBinaryExprNode($1, new cOpNode('='), $3);
                                    PROP_ERROR();
                                }
        |   expr '>' addit
                                { 
                                    $$ = new cBinaryExprNode($1, new cOpNode('>'), $3);
                                    PROP_ERROR();
                                }
        |   expr '<' addit
                                { 
                                    $$ = new cBinaryExprNode($1, new cOpNode('<'), $3);
                                    PROP_ERROR();
                                }
        |   expr LE addit
                                { 
                                    $$ = new cBinaryExprNode($1, new cOpNode(LE), $3);
                                    PROP_ERROR();
                                }
        |   expr GE addit
                                { 
                                    $$ = new cBinaryExprNode($1, new cOpNode(GE), $3);
                                    PROP_ERROR();
                                }
        |   expr NOT_EQUAL addit
                                { 
                                    $$ = new cBinaryExprNode($1, new cOpNode(NOT_EQUAL), $3);
                                    PROP_ERROR();
                                }
        |   addit
                                { $$ = $1; }

addit:      addit '+' term
                                {
                                    $$ = new cBinaryExprNode($1, new cOpNode('+'), $3);
                                    PROP_ERROR();
                                }
        |   addit '-' term
                                {
                                    $$ = new cBinaryExprNode($1, new cOpNode('-'), $3);
                                    PROP_ERROR();
                                }
        |   addit OR term
                                { 
                                    $$ = new cBinaryExprNode($1, new cOpNode(OR), $3); 
                                    PROP_ERROR();
                                }
        |   term
                                { $$ = $1; }
        |   '-' term
                                { $$ = new cUnaryExprNode(new cOpNode('-'), $2); CHECK_ERROR(); }

term:       term '*' fact
                                {
                                    $$ = new cBinaryExprNode($1, new cOpNode('*'), $3); 
                                    PROP_ERROR();
                                }
        |   term '/' fact
                                {
                                    $$ = new cBinaryExprNode($1, new cOpNode('/'), $3);
                                    PROP_ERROR();
                                }
        |   term MOD fact
                                { 
                                    $$ = new cBinaryExprNode($1, new cOpNode(MOD), $3);
                                    PROP_ERROR();
                                }
        |   term DIV fact
                                { 
                                    $$ = new cBinaryExprNode($1, new cOpNode(DIV), $3); 
                                    PROP_ERROR();
                                }
        |   term AND fact
                                { 
                                    $$ = new cBinaryExprNode($1, new cOpNode(AND), $3);
                                    PROP_ERROR();
                                }
        |   fact
                                { $$ = $1; }

fact:        '(' expr ')'
                                { $$ = $2; CHECK_ERROR(); }
        |   INT_VAL
                                { $$ = new cIntExprNode($1); CHECK_ERROR(); }
        |   REAL_VAL
                                { $$ = new cRealExprNode($1); CHECK_ERROR(); }
        |   variable
                                { $$ = $1; CHECK_ERROR(); }
        |   func_call
                                { $$ = $1; }
        |   NOT fact
                                { $$ = new cUnaryExprNode(new cOpNode(NOT), $2); CHECK_ERROR(); }

%%

// Function to format error messages
int yyerror(const char *msg)
{
    std::cout << "ERROR: " << msg << " at symbol "
        << yytext << " on line " << yylineno << "\n";

    return 0;
}
// Function gets called when a semantic error happens
void SemanticParseError(std::string error)
{
    std::cout << "ERROR: " << error << " on line " << yylineno << "\n";
    g_semanticErrorHappened = true;
    yynerrs++;
}
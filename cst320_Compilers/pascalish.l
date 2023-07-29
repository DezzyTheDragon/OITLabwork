%{
//**************************************
// lang.l
//
// Scanner definition file for CST 320 project
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

//#include "tokens.h"
#include "astnodes.h"
#include "pascalishparse.h"
#include "cSymbolTable.h"
#include "lex.h"

//#define DEBUG
#ifdef DEBUG
#define DO_RETURN(a) return Return(a)
#else
#define DO_RETURN(a) return (a)
#endif

int Return(int val);
int Process_INT(char* val);
int Process_REAL(char* val);
int Process_ID(char* name);

%}

%option noyywrap
%option noinput
%option nounput
%option yylineno


    /* definitions go here. PUNCTUATION is an example. */
PUNCTUATION          [.?;!]

%%
    /*Punctuation*/
"("                         DO_RETURN('(');
")"                         DO_RETURN(')');
"["                         DO_RETURN('[');
"]"                         DO_RETURN(']');
";"                         DO_RETURN(';');
","                         DO_RETURN(',');
"."                         DO_RETURN('.');
"+"                         DO_RETURN('+');
"-"                         DO_RETURN('-');
"*"                         DO_RETURN('*');
"/"                         DO_RETURN('/');
"="                         DO_RETURN('=');
"^"                         DO_RETURN('^');
"<"                         DO_RETURN('<');
">"                         DO_RETURN('>');
":"                         DO_RETURN(':');

    /*Operations*/
"<="                        DO_RETURN(LE);
">="                        DO_RETURN(GE);
":="                        DO_RETURN(ASSIGN);
"<>"                        DO_RETURN(NOT_EQUAL);

    /*Keywords*/

    /*Others*/
[A-Za-z]+[0-9a-zA-Z]*           DO_RETURN(Process_ID(yytext));
[0-9]+                          DO_RETURN(Process_INT(yytext)); //INT_VAL
[0-9]+"."[0-9]+                 DO_RETURN(Process_REAL(yytext));
"{".*"}"                        {};
[ \n\t]                         {};
.                               DO_RETURN(JUNK_TOKEN);


%%

// This function allows us to do extra processing on each token
int Return(int val)
{
    printf("Scanned '%s': %d\n", yytext, val);
    return val;
}

int Process_INT(char* val)
{
    yylval.int_val = atoi(val);
    return INT_VAL;
}

int Process_REAL(char* val)
{
    yylval.real_val = atof(val);
    return REAL_VAL;
}

int Process_ID(char* name)
{
    char* lowercaseName = name;

    for(int i = 0; i < strlen(lowercaseName); i++)
    {
        lowercaseName[i] = tolower(lowercaseName[i]);
    }

    cSymbol* temp = g_symbolTable.GlobalLookup(name);
    if(temp != nullptr)
    {
        yylval.symbol = temp;
    }
    else
    {
        cSymbol* newSym = new cSymbol(name);
        //g_symbolTable.Insert(newSym);
        yylval.symbol = newSym;
    }

    return yylval.symbol->GetTokenType();
}


//#pragma once
#include "initSymbolTable.h"
#include "astnodes.h"
#include "cSymbolTable.h"
#include "cSymbol.h"
//#include "tokens.h"
#include "pascalishparse.h"

void initSymbolTable::InitSymbolTable(cSymbolTable *symTable)
{
            cSymbol* keyword = new cSymbol("program", PROGRAM);
            symTable->Insert(keyword);
            keyword = new cSymbol("procedure", PROCEDURE);
            symTable->Insert(keyword);
            keyword = new cSymbol("function", FUNCTION);
            symTable->Insert(keyword);
            keyword = new cSymbol("declare", DECLARE);
            symTable->Insert(keyword);
            keyword = new cSymbol("begin", OPEN);
            symTable->Insert(keyword);
            keyword = new cSymbol("end", CLOSE);
            symTable->Insert(keyword);
            keyword = new cSymbol("if", IF);
            symTable->Insert(keyword);
            keyword = new cSymbol("then", THEN);
            symTable->Insert(keyword);
            keyword = new cSymbol("else", ELSE);
            symTable->Insert(keyword);
            keyword = new cSymbol("while", WHILE);
            symTable->Insert(keyword);
            keyword = new cSymbol("repeat", REPEAT);
            symTable->Insert(keyword);
            keyword = new cSymbol("until", UNTIL);
            symTable->Insert(keyword);
            keyword = new cSymbol("for", FOR);
            symTable->Insert(keyword);
            keyword = new cSymbol("do", DO);
            symTable->Insert(keyword);
            keyword = new cSymbol("to", TO);
            symTable->Insert(keyword);
            keyword = new cSymbol("downto", DOWNTO);
            symTable->Insert(keyword);
            keyword = new cSymbol("forward", FORWARD);
            symTable->Insert(keyword);
            keyword = new cSymbol("const", CONST);
            symTable->Insert(keyword);
            keyword = new cSymbol("type", TYPE);
            symTable->Insert(keyword);
            keyword = new cSymbol("var", VAR);
            symTable->Insert(keyword);
            keyword = new cSymbol("array", ARRAY);
            symTable->Insert(keyword);
            keyword = new cSymbol("record", RECORD);
            symTable->Insert(keyword);
            keyword = new cSymbol("of", OF);
            symTable->Insert(keyword);
            keyword = new cSymbol("nil", NIL);
            symTable->Insert(keyword);

            keyword = new cSymbol("integer", TYPE_ID);
            keyword->SetDecl( new cBaseTypeNode("integer", 4, false));
            symTable->Insert(keyword);
            
            keyword = new cSymbol("LONGINT", TYPE_ID);
            keyword->SetDecl( new cBaseTypeNode("LONGINT", 8, false));
            symTable->Insert(keyword);
            
            keyword = new cSymbol("real", TYPE_ID);
            keyword->SetDecl( new cBaseTypeNode("real", 4, true));
            symTable->Insert(keyword);
            
            keyword = new cSymbol("DOUBLE", TYPE_ID);
            keyword->SetDecl( new cBaseTypeNode("DOUBLE", 8, true));
            symTable->Insert(keyword);
            
            keyword = new cSymbol("char", TYPE_ID);
            keyword->SetDecl( new cBaseTypeNode("char", 1, false) );
            symTable->Insert(keyword);
            
            keyword = new cSymbol("or", OR);
            symTable->Insert(keyword);
            keyword = new cSymbol("div", DIV);
            symTable->Insert(keyword);
            keyword = new cSymbol("mod", MOD);
            symTable->Insert(keyword);
            keyword = new cSymbol("and", AND);
            symTable->Insert(keyword);
            keyword = new cSymbol("not", NOT);
            symTable->Insert(keyword);
            keyword = new cSymbol("write", WRITE);
            symTable->Insert(keyword);
}


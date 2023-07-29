#pragma once

#include "cDeclNode.h"
#include "cSymbol.h"
#include "cExprNode.h"

extern cSymbolTable g_symbolTable;

class cConstDeclNode : public cDeclNode
{
    public:
        cConstDeclNode(cSymbol* name, cExprNode* value) : cDeclNode()
        {
            cSymbol* localSym = g_symbolTable.LocalLookup(name->GetName());
            if(localSym != nullptr)
            {
                string errorMsg = "Symbol " + localSym->GetName() + 
                            " already exists in current scope";
                SemanticParseError(errorMsg);
            }

            cSymbol* temp = g_symbolTable.GlobalLookup(name->GetName());
            if(temp != nullptr)
            {
                name = new cSymbol(name->GetName());
            }
            AddChild(name);
            AddChild(value);
            name->SetDecl(this);
            g_symbolTable.Insert(name);
        }
        virtual string NodeType() {return "const_decl"; }
        virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};

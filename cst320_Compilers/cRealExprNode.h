#pragma once

#include <string>
#include "cAstNode.h"
#include "cExprNode.h"
#include "cSymbolTable.h"

extern cSymbolTable g_symbolTable;

class cRealExprNode : public cExprNode
{
    public:
        cRealExprNode(float val) : cExprNode()
        {
            m_val = val;
        }
        virtual string AttributesToString()
        {
            return " value=\"" + std::to_string(m_val) + "\"";
        }
        virtual string NodeType()
        {
            return "REAL";
        }
        virtual void Visit (cVisitor *visitor) { visitor->Visit(this); }
        virtual cDeclNode *GetType() 
        { 
            cSymbol *symbol = g_symbolTable.GlobalLookup("real");
            return symbol->GetDecl();
        }
    protected:
        float m_val;
};

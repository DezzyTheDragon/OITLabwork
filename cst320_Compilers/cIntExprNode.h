#pragma once
//**************************************
// cIntExprNode.h
//
// Defines an AST node for an integer constant (literals).
//
// Inherits from cExprNode so that integer constants can be used anywhere 
// expressions are used.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cExprNode.h"
#include "cSymbolTable.h"

extern cSymbolTable g_symbolTable;

class cIntExprNode : public cExprNode
{
    public:
        // param is the value of the integer constant
        cIntExprNode(int value) : cExprNode()
        {
            m_value = value;
        }
        int GetValue() { return m_value; }
        virtual string AttributesToString() 
        {
            return " value=\"" + std::to_string(m_value) + "\"";
        }
        virtual string NodeType() { return string("INTEGER"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual cDeclNode *GetType() 
        {
            cSymbol *symbol = nullptr;
            if((-128 <= m_value) && ( m_value <= 127))
            {
                symbol = g_symbolTable.GlobalLookup("char");
            }
            else
            {
                symbol = g_symbolTable.GlobalLookup("integer");
            }
            return symbol->GetDecl();
        }
    protected:
        int m_value;        // value of integer constant (literal)
};

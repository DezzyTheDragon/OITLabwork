#pragma once

#include <string>
#include "cExprNode.h"
#include "cSymbol.h"

extern cSymbolTable g_symbolTable;

class cVarExprNode : public cExprNode
{
    public:
        cVarExprNode(cSymbol* sym) : cExprNode()
        {
            AddChild(sym);
        }
        cSymbol *GetName()
        {
            cSymbol *sym = dynamic_cast<cSymbol*>(GetChild(0));
            return sym;
        }
        void SetOffset(int offset) { m_offset = offset; }
        void SetSize(int size) { m_size = size; }
        int GetOffset() { return m_offset; }
        int GetSize() { return m_size; }
        virtual string NodeType() { return "varref"; }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual cDeclNode *GetType()
        {
            cSymbol* temp = dynamic_cast<cSymbol*>(GetChild(0));
            return temp->GetDecl();
        }
        virtual string AttributesToString()
        {
            //cSymbol *sym = dynamic_cast<cSymbol*>(GetChild(0));
            //string ret = " size=\"" + std::to_string(sym->GetDecl()->GetSize()) + "\"";
            //ret += " offset=\"" + std::to_string(sym->GetDecl()->GetOffset()) + "\"";
            string ret = " size=\"" + std::to_string(m_size) + "\"";
            ret += " offset=\"" + std::to_string(m_offset) + "\"";
            return ret;
        }
    private:
        int m_offset = 0;
        int m_size = 0;
};

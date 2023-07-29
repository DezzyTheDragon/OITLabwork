#pragma once

#include <string>
#include "cDeclNode.h"
#include "cSymbol.h"
#include "cSymbolTable.h"

extern cSymbolTable g_symbolTable;

class cBaseTypeNode : public cDeclNode
{
    public:
        cBaseTypeNode(cSymbol* name, cSymbol* type, int size) : cDeclNode()
        {
            cSymbol* localSym = g_symbolTable.LocalLookup(name->GetName());
            if(localSym != nullptr)
            {
                string errorMsg = "Symbol " + localSym->GetName() + 
                            " already exists in current scope";
                SemanticParseError(errorMsg);
            }

            m_name = name->GetName();
            m_isReal = type->GetDecl()->IsReal();
            m_size = size;
            //add to sym table and set decl in this constructor
            name->SetTokenType(type->GetTokenType());
            name->SetDecl(this);
            g_symbolTable.Insert(name);
        }
        cBaseTypeNode(string name, int size, bool isReal) : cDeclNode()
        {
            m_name = name;
            m_size = size;
            m_isReal = isReal;
        }
        virtual string GetTypeName()
        {
            return m_name;
        }
        virtual string AttributesToString()
        {
            string ret =  " name=\"" + m_name + "\"";
            ret += " size=\"" + std::to_string(m_size) + "\"";
            ret += " isFloat=\"" + std::to_string(m_isReal) + "\"";
            return ret;
        }
        virtual string NodeType() { return "type"; }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual bool IsReal() { return m_isReal; }
        virtual bool IsInt() { return !m_isReal; }
    protected:
        string m_name;
        bool m_isReal;
};

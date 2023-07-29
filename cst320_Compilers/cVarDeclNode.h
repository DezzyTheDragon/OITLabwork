#pragma once

#include "cAstNode.h"
#include "cSymbol.h"
#include "cSymbolTable.h"

extern cSymbolTable g_symbolTable;

class cVarDeclNode : public cDeclNode
{
    public:
        cVarDeclNode(cSymbol* name, cSymbol* type) : cDeclNode()
        {
            //check if the symbol already exists localy
            cSymbol* localSym = g_symbolTable.LocalLookup(name->GetName());
            if(localSym != nullptr)
            {
                string errorMsg = "Symbol " + localSym->GetName() + 
                            " already exists in current scope";
                SemanticParseError(errorMsg);
            }
            
            //check if in table, if not create new and add it
            cSymbol* temp = g_symbolTable.GlobalLookup(name->GetName());
            if(temp != nullptr)
            {
                name = new cSymbol(name->GetName());
            }

            AddChild(type->GetDecl());
            AddChild(name);
            name->SetDecl(this);
            g_symbolTable.Insert(name);
        }
        virtual string NodeType() { return "var_decl"; }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual string AttributesToString()
        {
            string ret = " size=\"" + std::to_string(m_size) + "\"";
            ret += " offset=\"" + std::to_string(m_offset) + "\"";
            return ret;
        }
        virtual cDeclNode *GetType()
        {
            return dynamic_cast<cDeclNode*>(GetChild(0)); 
        }
        virtual string GetTypeName() 
        {
            //cDeclNode *decl = dynamic_cast<cDeclNode*>(GetChild(0));
            cDeclNode *decl = dynamic_cast<cDeclNode*>(GetChild(0));
            return decl->GetTypeName(); 
        }
        virtual int GetTypeSize()
        {
            //cDeclNode *decl = dynamic_cast<cDeclNode*>(GetChild(0));
            cDeclNode *decl = dynamic_cast<cDeclNode*>(GetChild(0));
            return decl->GetTypeSize();
        }
        virtual bool IsReal()
        {
            return dynamic_cast<cDeclNode*>(GetChild(0))->IsReal();
        }
        virtual bool IsInt()
        {
            return dynamic_cast<cDeclNode*>(GetChild(0))->IsInt();
        }
        virtual bool IsFunc()
        {
            return dynamic_cast<cDeclNode*>(GetChild(0))->IsFunc();
        }
        virtual bool IsProc()
        {
            return dynamic_cast<cDeclNode*>(GetChild(0))->IsProc();
        }
};

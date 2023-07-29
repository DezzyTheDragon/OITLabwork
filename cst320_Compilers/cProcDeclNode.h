#pragma once

#include "cDeclNode.h"
#include "cSymbol.h"
#include "cVarDeclsNode.h"
#include "cBlockNode.h"

extern cSymbolTable g_symbolTable;

class cProcDeclNode : public cDeclNode
{
    public:
        cProcDeclNode() : cDeclNode()
        {
            
        }
        cProcDeclNode(cSymbol* name, cVarDeclsNode* params, cBlockNode* block) : cDeclNode()
        {
            AddChild(name);
            AddChild(params);
            AddChild(block);
        }
        void AddName(cSymbol* name) 
        { 
            cSymbol* temp = g_symbolTable.GlobalLookup(name->GetName());
            if(temp != nullptr)
            {
                name = new cSymbol(name->GetName());
            }
            AddChild(name);
            SetChild(0, name);
            name->SetDecl(this);
            g_symbolTable.Insert(name);
        }
        string GetName()
        {
            return dynamic_cast<cSymbol*>(GetChild(0))->GetName();
        }
        cVarDeclsNode *GetParams()
        {
            cVarDeclsNode *params = dynamic_cast<cVarDeclsNode*>(GetChild(1));
            return params;
        }
        cBlockNode *GetBlock()
        {
            cBlockNode *block = dynamic_cast<cBlockNode*>(GetChild(2));
            return block;
        }
        void AddParams(cVarDeclsNode* params) { AddChild(params); SetChild(1, params); }
        void AddBlock(cBlockNode* block) { AddChild(block); SetChild(2, block); }
        virtual bool IsProc() { return true; }
        virtual string NodeType() { return "proc"; }
        virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};

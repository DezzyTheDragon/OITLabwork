#pragma once

#include "cAstNode.h"
#include "cSymbol.h"

class cIdListNode : public cAstNode
{
    public:
        cIdListNode(cSymbol *sym) : cAstNode()
        {
            AddChild(sym);
        }
        void AddSym(cSymbol *sym)
        {
            AddChild(sym);
        }
        int GetChildCount()
        {
            return NumChildren(); 
        }
        cSymbol* GetChildAt(int index)
        {
            return dynamic_cast<cSymbol*>(GetChild(index));
        }
        virtual string NodeType() {return "IdList"; }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};

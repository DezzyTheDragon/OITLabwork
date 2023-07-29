#pragma once

#include "cExprNode.h"
#include "cSymbol.h"
#include "cExprsNode.h"

class cFuncExprNode : public cExprNode
{
    public:
        cFuncExprNode(cSymbol* name, cExprsNode* expr) : cExprNode()
        {
            AddChild(name);
            AddChild(expr);
        }
        virtual string NodeType() { return "funcCall"; }
        virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
        virtual cDeclNode *GetType() 
        { 
            cSymbol *sym = dynamic_cast<cSymbol*>(GetChild(0));
            return dynamic_cast<cDeclNode*>(sym->GetDecl()); 
        }

        virtual cSymbol* GetName() 
        {
            cSymbol *sym = dynamic_cast<cSymbol*>(GetChild(0)); 
            return sym;
        }

        cExprsNode *GetParams() { return dynamic_cast<cExprsNode*>(GetChild(1)); }

        virtual int ExprsNum()
        {
            int count = 0;
            cExprsNode *exprs = dynamic_cast<cExprsNode*>(GetChild(1));
            if(exprs != nullptr)
            {
                count = exprs->GetExprCount();
            }
            return count;
        }
        void SetParamSize(int size) { m_paramSize = size; }
        int GetParamSize() { return m_paramSize; }
        virtual string AttributesToString()
        {
            //if(m_paramSize > 0)
            //{
                return " paramSize=\"" + std::to_string(m_paramSize) + "\"";
            //}
            //return "";
        }
        /*
        virtual cExprsNode* GetExprs()
        {
            cExprsNode *exprs = dynamic_cast<cExprsNode*>(GetChild(1));
            return exprs;
        }
        */
    private:
        int m_paramSize = 0;
};

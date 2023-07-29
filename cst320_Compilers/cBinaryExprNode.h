#pragma once

#include "cAstNode.h"
#include "cExprNode.h"
#include "cOpNode.h"


class cBinaryExprNode : public cExprNode
{
    public:
        cBinaryExprNode(cExprNode *valA, cOpNode *op, cExprNode *valB) : cExprNode()
        {
            AddChild(valA);
            AddChild(op);
            AddChild(valB);
        }
        cExprNode* GetLExpr() { return dynamic_cast<cExprNode*>(GetChild(0)); }
        cExprNode* GetRExpr() { return dynamic_cast<cExprNode*>(GetChild(2)); }
        cOpNode* GetOp() { return dynamic_cast<cOpNode*>(GetChild(1)); }
        virtual string NodeType() { return "expr"; }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual cDeclNode *GetType() 
        { 
            cExprNode *lExpr = dynamic_cast<cExprNode*>(GetChild(0));
            cExprNode *rExpr = dynamic_cast<cExprNode*>(GetChild(2));
            
            if(lExpr->GetType()->IsReal() == rExpr->GetType()->IsReal())
            {
                if(lExpr->GetType()->GetTypeSize() > rExpr->GetType()->GetTypeSize())
                {
                    return lExpr->GetType();
                }
                else
                {
                    return rExpr->GetType();
                }
            }
            else if(rExpr->GetType()->IsReal())
            {
                return rExpr->GetType();
            }
            else
            {
                return lExpr->GetType();
            }
        }
};

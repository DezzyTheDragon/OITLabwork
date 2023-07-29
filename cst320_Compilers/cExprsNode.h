#pragma once

#include "cAstNode.h"
#include "cExprNode.h"

class cExprsNode : public cAstNode
{
    public:
    cExprsNode(cExprNode *exprNode) : cAstNode()
    {
        AddChild(exprNode);
    }
    void AddExpr(cExprNode *exprNode)
    {
        AddChild(exprNode);
    }
    cExprNode* GetExpr(int index)
    {
        return dynamic_cast<cExprNode*>(GetChild(index));
    }
    virtual int GetExprCount() { return NumChildren(); }
    virtual string NodeType() { return "exprList"; };
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); };
};

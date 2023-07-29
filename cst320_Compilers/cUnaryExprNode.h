#pragma once

#include "cExprNode.h"
#include "cOpNode.h"
#include "cExprNode.h"

class cUnaryExprNode : public cExprNode
{
    public:
        cUnaryExprNode(cOpNode* op, cExprNode* expr) : cExprNode()
        {
            AddChild(op);
            AddChild(expr);
        }
        virtual string NodeType() { return "unaryExpr"; }
        virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
        virtual cDeclNode *GetType() 
        { 
            cSymbol* sym = dynamic_cast<cSymbol*>(GetChild(0));
            return sym->GetDecl(); 
        }
};

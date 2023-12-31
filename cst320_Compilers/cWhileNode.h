#pragma once

#include "cStmtNode.h"
#include "cExprNode.h"

class cWhileNode : public cStmtNode
{
    public:
        cWhileNode(cExprNode* expr, cStmtNode* stmt) : cStmtNode()
        {
            AddChild(expr);
            AddChild(stmt);
        }
        virtual string NodeType() { return "while"; }
        virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};

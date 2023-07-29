#pragma once

#include "cStmtNode.h"
#include "cExprNode.h"

class cIfNode : public cStmtNode
{
    public:
        cIfNode(cExprNode* cond, cStmtNode* ifStmt) : cStmtNode()
        {
            AddChild(cond);
            AddChild(ifStmt);
        }
        cIfNode(cExprNode* cond, cStmtNode* ifStmt, cStmtNode* elseStmt) : cStmtNode()
        {
            AddChild(cond);
            AddChild(ifStmt);
            AddChild(elseStmt);
        }
        virtual string NodeType() { return "if"; }
        virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};

#pragma once

#include "cStmtNode.h"
#include "cExprsNode.h"

class cWriteNode : public cStmtNode
{
    public:
        cWriteNode(cExprsNode *exprs) : cStmtNode()
        {
            AddChild(exprs);
        }
        int GetParamCount() 
        { 
            cExprsNode *exprs = dynamic_cast<cExprsNode*>(GetChild(0));
            return exprs->GetExprCount(); }
        virtual string NodeType() { return "WRITE"; };
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); };
};

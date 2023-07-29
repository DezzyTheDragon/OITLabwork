#pragma once

#include "cVisitor.h"

class cSemantics : public cVisitor
{
    public:
        cSemantics();
        virtual void VisitAllNodes(cAstNode *node);

        virtual void Visit(cFuncExprNode *node);
        virtual void Visit(cVarExprNode *node);
        virtual void Visit(cProcCallNode *node);
        virtual void Visit(cAssignNode *node);
};




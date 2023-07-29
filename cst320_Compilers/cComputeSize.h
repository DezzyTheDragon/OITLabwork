#pragma once

#include "cVisitor.h"

class cComputeSize : public cVisitor
{
    public:
        cComputeSize();
        virtual void VisitAllNodes(cAstNode *node);

        virtual void Visit(cProgramNode *node);
        virtual void Visit(cVarDeclNode *node);
        virtual void Visit(cVarDeclsNode *node);
        virtual void Visit(cBlockNode *node);
        virtual void Visit(cFuncDeclNode *node);
        virtual void Visit(cProcDeclNode *node);
        virtual void Visit(cFuncExprNode *node);
        virtual void Visit(cProcCallNode *node);
        virtual void Visit(cVarExprNode *node);
    protected:
    private:
        int roundToWord(int round);
        void checkHighWater();
    int m_offset = 0;
    int m_highWater = 0;
    bool m_inFunc = false;
    bool m_inProc = false;
    int m_blockLayer = 0;
};

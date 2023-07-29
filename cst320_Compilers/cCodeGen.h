#pragma once

#include <string>
#include "cVisitor.h"

class cCodeGen : public cVisitor
{
    public:
        cCodeGen(std::string filename);
        ~cCodeGen();
        virtual void VisitAllNodes(cAstNode *node);

        virtual void Visit(cProgramNode *node);
        virtual void Visit(cBlockNode *node);
        virtual void Visit(cWriteNode *node);
        virtual void Visit(cExprsNode *node);
        virtual void Visit(cBinaryExprNode *node);
        virtual void Visit(cOpNode *node);
        virtual void Visit(cAssignNode *node);
        virtual void Visit(cIntExprNode *node);
        virtual void Visit(cVarExprNode *node);
        virtual void Visit(cProcDeclNode *node);
        virtual void Visit(cProcCallNode *node);
        virtual void Visit(cFuncDeclNode *node);
        virtual void Visit(cFuncExprNode *node);

    private:
    bool m_reverse = false;
    bool m_isLExpr = false;
};

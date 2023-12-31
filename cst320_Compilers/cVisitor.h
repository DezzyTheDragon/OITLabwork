#pragma once
//*************************************************
// Definition of Visitor base class
//
// Author: Philip Howard
// Email:  phil.howard@oit.edu
//

// Need forward declaration of all classes.
// Forward declare them to avoid circular include problems

class cAstNode;
class cBlockNode;
class cDeclNode;
class cDeclsNode;
class cExprNode;
class cIntExprNode;
class cNilNode;
class cOpNode;
class cProgramNode;
class cStmtNode;
class cStmtsNode;
class cSymbol;

class cAssignNode;
class cBaseTypeNode;
class cBinaryExprNode;
class cConstDeclNode;
class cExprsNode;
class cFuncDeclNode;
class cFuncExprNode;
class cIdListNode;
class cIfNode;
class cProcCallNode;
class cProcDeclNode;
class cRealExprNode;
class cUnaryExprNode;
class cVarDeclNode;
class cVarDeclsNode;
class cVarExprNode;
class cWhileNode;
class cWriteNode;


class cVisitor
{
    public:
        cVisitor() {}

        // visitor implementations must define the visit order
        // This will usually just be a call to VisitAllChildren()
        virtual void VisitAllNodes(cAstNode *node) = 0;

        // A visit method for every AST node
        virtual void Visit(cAstNode *node);
        virtual void Visit(cBlockNode *node);
        virtual void Visit(cDeclNode *node);
        virtual void Visit(cDeclsNode *node);
        virtual void Visit(cExprNode *node);
        virtual void Visit(cIntExprNode *node);
        virtual void Visit(cNilNode *node);
        virtual void Visit(cOpNode *node);
        virtual void Visit(cProgramNode *node);
        virtual void Visit(cStmtNode *node);
        virtual void Visit(cStmtsNode *node);
        virtual void Visit(cSymbol *node);
        
        virtual void Visit(cAssignNode *node);
        virtual void Visit(cBaseTypeNode *node);
        virtual void Visit(cBinaryExprNode *node);
        virtual void Visit(cConstDeclNode *node);
        virtual void Visit(cExprsNode *node);
        virtual void Visit(cFuncDeclNode *node);
        virtual void Visit(cFuncExprNode *node);
        virtual void Visit(cIdListNode *node);
        virtual void Visit(cIfNode *node);
        virtual void Visit(cProcCallNode *node);
        virtual void Visit(cProcDeclNode *node);
        virtual void Visit(cRealExprNode *node);
        virtual void Visit(cUnaryExprNode *node);
        virtual void Visit(cVarDeclNode *node);
        virtual void Visit(cVarDeclsNode *node);
        virtual void Visit(cVarExprNode *node);
        virtual void Visit(cWhileNode *node);
        virtual void Visit(cWriteNode *node);
        
};

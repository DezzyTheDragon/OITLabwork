#include "cCodeGen.h"
#include "astnodes.h"
#include "pascalishparse.h"
#include "emit.h"

cCodeGen::cCodeGen(std::string filename) : cVisitor()
{
    if(!InitOutput(filename))
    {
        std::cerr << "Failed to open file: " + filename + "\n";
        exit(-1);
    }
}

cCodeGen::~cCodeGen()
{
    if(!FinalizeOutput())
    {
        std::cerr << "Failed to finish writing to output\n";
        exit(-1);
    }
}

void cCodeGen::VisitAllNodes(cAstNode *node)
{
    node->Visit(this);
}

void cCodeGen::Visit(cProgramNode *node)
{
    cDeclsNode *decls = node->GetBlock()->GetDecls();
    if(decls != nullptr)
    {
        decls->VisitAllChildren(this);
    }
    EmitString(".function main\n");
    EmitString("main:\n");
    node->GetBlock()->Visit(this);
    EmitString("PUSH 0\n");
    EmitString("RETURNV\n");
    //node->VisitAllChildren(this);
}

void cCodeGen::Visit(cBlockNode *node)
{
    EmitString("ADJSP ");
    EmitInt(node->GetSize());
    EmitString("\n");
    node->GetStmts()->VisitAllChildren(this);
}

void cCodeGen::Visit(cWriteNode *node)
{
    //get however many arguments there are and loop printing and poping each one
    m_reverse = true;
    node->VisitAllChildren(this);
    m_reverse = false;
    int count = node->GetParamCount();
    for(int i = 0; i < count; i++)
    {
        EmitString("CALL @print\n");
        EmitString("POP\n");
        EmitString("POP\n");
    }
}

void cCodeGen::Visit(cExprsNode *node)
{
    if(m_reverse)
    {
        int count = node->GetExprCount();
        for(int i = count; i > 0; i--)
        {
            node->GetExpr((i - 1))->Visit(this);
        }
    }
    else
    {
        node->VisitAllChildren(this);
    }
}

void cCodeGen::Visit(cBinaryExprNode *node)
{
    node->GetLExpr()->Visit(this);
    node->GetRExpr()->Visit(this);
    node->GetOp()->Visit(this);
}

void cCodeGen::Visit(cOpNode *node)
{
    int op = node->GetOp();
    switch(op)
    {
        case '+':
            EmitString("PLUS\n");
            break;
        case '-':
            EmitString("MINUS\n");
            break;
        case '*':
            EmitString("TIMES\n");
            break;
        case '/':
            EmitString("DIVIDE\n");
            break;
        default:
            exit(-1);
            break;
    }
}

void cCodeGen::Visit(cAssignNode *node)
{
    node->GetRight()->Visit(this);
    m_isLExpr = true;
    node->GetLeft()->Visit(this);
    m_isLExpr = false;
}

void cCodeGen::Visit(cIntExprNode *node)
{
    EmitString("PUSH ");
    EmitInt(node->GetValue());
    EmitString("\n");
}

void cCodeGen::Visit(cVarExprNode *node)
{
    if(m_isLExpr)
    {
        EmitString("POPVAR ");
    }
    else
    {
        EmitString("PUSHVAR ");
    }
    EmitInt(node->GetOffset());
    EmitString("\n");
}

void cCodeGen::Visit(cProcDeclNode *node)
{
    cDeclsNode *decls = node->GetBlock()->GetDecls();
    if(decls != nullptr)
    {
        decls->VisitAllChildren(this);
    }
    EmitString(".function ");
    EmitString(node->GetName() + "\n");
    EmitString(node->GetName() + ":\n");
    node->GetBlock()->Visit(this);
    EmitString("PUSH 0\n");
    EmitString("RETURN\n");
}

void cCodeGen::Visit(cProcCallNode *node)
{
    m_reverse = true;
    node->GetParams()->Visit(this);
    m_reverse = false;
    EmitString("CALL @");
    EmitString(node->GetName()->GetName());
    EmitString("\n");
    int paramCount = node->GetParamSize() / 4;
    for(int i = 0; i < paramCount; i++)
    {
        EmitString("POP\n");
    }
}

void cCodeGen::Visit(cFuncDeclNode *node)
{
    cDeclsNode *decls = node->GetBlock()->GetDecls();
    if(decls != nullptr)
    {
        decls->VisitAllChildren(this);
    }
    EmitString(".function ");
    EmitString(node->GetName()->GetName() + "\n");
    EmitString(node->GetName()->GetName() + ":\n");
    node->GetBlock()->Visit(this);
    EmitString("PUSHVAR 0\n");
    EmitString("RETURNV\n");

}

void cCodeGen::Visit(cFuncExprNode *node)
{
    m_reverse = true;
    node->GetParams()->Visit(this);
    m_reverse = false;
    EmitString("CALL @");
    EmitString(node->GetName()->GetName());
    EmitString("\n");
    EmitString("POPARGS ");
    EmitInt(node->GetParamSize());
    EmitString("\n");
}
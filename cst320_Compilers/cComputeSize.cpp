#include "cComputeSize.h"
#include "astnodes.h"

cComputeSize::cComputeSize() : cVisitor()
{}

void cComputeSize::VisitAllNodes(cAstNode *node)
{
    node->Visit(this);
}

void cComputeSize::Visit(cProgramNode *node)
{
    //m_inMainBlock = true;
    node->VisitAllChildren(this);
    //m_inMainBlock = false;
}

void cComputeSize::Visit(cVarDeclNode *node)
{
    node->SetSize(node->GetType()->GetSize());
    if(m_offset >= 0)
    {
        if(node->GetSize() == 1)
        {
            node->SetOffset(m_offset);
        }
        else
        {
            m_offset = roundToWord(m_offset);
            node->SetOffset(m_offset);
        }
        m_offset += node->GetSize();
    }
    else
    {
        m_offset = roundToWord(m_offset);
        node->SetOffset(m_offset);
        m_offset -= node->GetSize();
    }
    /*
    if(m_offset >= 0)
    {
        m_offset += node->GetSize();
    }
    else
    {
        m_offset -= node->GetSize();
    }
    */
    if(!m_inFunc && !m_inProc)
    {
        if (m_offset > m_highWater) m_highWater = m_offset;
    }
}

void cComputeSize::Visit(cVarDeclsNode *node)
{
    int startOffset = m_offset;
    node->VisitAllChildren(this);
    int endOffset = m_offset;
    //int size = std::abs(endOffset) - std::abs(startOffset);
    int size = std::abs(endOffset - startOffset);
    node->SetSize(roundToWord(size));
}

void cComputeSize::Visit(cBlockNode *node)
{
    int startOffset = 0;
    if(!m_inFunc)
    {
        startOffset = m_offset;
    }
    m_blockLayer++;
    node->VisitAllChildren(this);
    m_blockLayer--;
    int endOffset = m_offset;
    //int size = std::abs(endOffset) - std::abs(startOffset);
    int size = std::abs(endOffset - startOffset);
    if(m_blockLayer > 0)
    {
        node->SetSize(roundToWord(size));
    }
    else
    {
        node->SetSize(roundToWord(m_highWater));
    }
    m_offset = startOffset;
    //node->SetSize(roundToWord(m_highWater));
}

void cComputeSize::Visit(cFuncDeclNode *node)
{
    int oldOffset = m_offset;
    //set params
    cVarDeclsNode *params = node->GetParams();
    m_offset = -12;
    params->Visit(this);
    //set func
    m_offset = 0;
    node->SetSize(node->GetType()->GetSize());
    node->SetOffset(m_offset);
    m_offset += node->GetSize();
    //set block
    cBlockNode *block = node->GetBlock();
    m_inFunc = true;
    block->Visit(this);
    m_inFunc = false;
    m_offset = oldOffset;
}

void cComputeSize::Visit(cProcDeclNode *node)
{
    int oldOffset = m_offset;
    //set params
    cVarDeclsNode *params = node->GetParams();
    m_offset = -12;
    params->Visit(this);
    //set proc
    m_offset = 0;
    cBlockNode *block = node->GetBlock();
    m_inProc = true;
    block->Visit(this);
    m_inProc = false;
    m_offset = oldOffset;

}

void cComputeSize::Visit(cFuncExprNode *node)
{
    cDeclNode *Decl = node->GetType();
    cFuncDeclNode *funcDecl = nullptr;
    int size = 0;
    if(Decl->IsFunc())
    {
        funcDecl = dynamic_cast<cFuncDeclNode*>(Decl);
        if(funcDecl->GetParams() != nullptr)
        {
            size = funcDecl->GetParams()->GetSize();
        }
    }
    node->SetParamSize(size);
    node->VisitAllChildren(this);
}

void cComputeSize::Visit(cProcCallNode *node)
{
    cDeclNode *decl = node->GetType();
    cProcDeclNode *procDecl = nullptr;
    int size = 0;
    if(decl->IsProc())
    {
        procDecl = dynamic_cast<cProcDeclNode*>(decl);
        if(procDecl->GetParams() != nullptr)
        {
            size = procDecl->GetParams()->GetSize();
        }
    }
    node->SetParamSize(size);
    node->VisitAllChildren(this);
}

void cComputeSize::Visit(cVarExprNode *node)
{
    node->SetSize(node->GetType()->GetSize());
    node->SetOffset(node->GetType()->GetOffset());
}

int cComputeSize::roundToWord(int round)
{
    //int add = 0;
    if(round != 0)
    {
        int add = std::abs(round % 4);
        if(add != 0)
        {
            if(round > 0)
            {
                round += (4 - add);
            }
            else
            {
                round -= (4 - add);
            }
        }
    }
    return round;
}

void cComputeSize::checkHighWater()
{
    if(roundToWord(m_offset) > m_highWater)
    {
        m_highWater = roundToWord(m_offset);
    }
}
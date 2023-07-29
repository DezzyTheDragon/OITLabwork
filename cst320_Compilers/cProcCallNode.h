#pragma once

#include "cStmtNode.h"
#include "cSymbol.h"
#include "cExprsNode.h"

class cProcCallNode : public cStmtNode
{
    public:
        cProcCallNode(cSymbol* name, cExprsNode* args) : cStmtNode()
        {
            AddChild(name);
            AddChild(args);
        }
        cSymbol *GetName()
        {
            cSymbol *sym = dynamic_cast<cSymbol*>(GetChild(0));
            return sym;
        }
        cDeclNode *GetType()
        {
            cSymbol *sym = dynamic_cast<cSymbol*>(GetChild(0));
            return sym->GetDecl();
        }
        cExprsNode *GetParams() { return dynamic_cast<cExprsNode*>(GetChild(1)); }
        void SetParamSize(int size)
        {
            m_paramSize = size;
        }
        int GetParamSize() { return m_paramSize; }
        virtual string AttributesToString()
        {
            //if(m_paramSize > 0)
            //{
                return " paramSize=\"" + std::to_string(m_paramSize) + "\"";
            //}
            //return "";
        }
        virtual string NodeType() { return "procCall"; }
        virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
    private:
        int m_paramSize = 0;
};

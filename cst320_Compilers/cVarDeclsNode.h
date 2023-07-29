#pragma once

#include "cDeclsNode.h"
#include "cIdListNode.h"
#include "cSymbol.h"
#include "cVarDeclNode.h"

class cVarDeclsNode : public cDeclsNode
{
    public:
        cVarDeclsNode(cIdListNode *list, cSymbol *type) : cDeclsNode()
        {
            //for every identifier in the list
            //add child as cVarDeclNode
            int count = list->GetChildCount();
            for(int i = 0; i < count; i++)
            {
                AddChild( new cVarDeclNode(list->GetChildAt(i), type));
            }
        }
        void AddAllId(cVarDeclsNode *list)
        {
            AddAllChildren(list);
        }
        int GetVarDeclCount()
        {
            return NumChildren();
        }
        virtual void SetSize(int size) { m_size = size; }
        virtual int GetSize() { return m_size; }
        virtual string AttributesToString()
        {
            return " size=\"" + std::to_string(m_size) + "\"";
        }
        virtual string NodeType() { return "var_decls"; }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    private:
        int m_size;
};

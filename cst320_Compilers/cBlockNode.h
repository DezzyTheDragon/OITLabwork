#pragma once
//**************************************
// cBlockNode.h
//
// Defines AST node for a block of code (stuff inside {})
//
// Since blocks can take the place of statements, this class inherits from 
// cStmtNode
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cDeclsNode.h"
#include "cStmtNode.h"
#include "cStmtsNode.h"

class cBlockNode : public cStmtNode
{
    public:
        // params are the decls and statements contained in the block
        cBlockNode(cDeclsNode *decls, cStmtsNode *statements)
            : cStmtNode()
        {
            AddChild(decls);
            AddChild(statements);
        }
        cDeclsNode *GetDecls() { return dynamic_cast<cDeclsNode*>(GetChild(0)); }
        cStmtsNode *GetStmts() { return dynamic_cast<cStmtsNode*>(GetChild(1)); }
        void SetSize(int size) { m_size = size; }
        int GetSize() { return m_size; }
        virtual string NodeType() { return string("block"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual string AttributesToString() 
        {
            string ret = "";
            if(m_size > 0)
            {
                ret = " size=\"" + std::to_string(m_size) + "\"";
            }
            return ret;
        }
    private:
        int m_size;
};

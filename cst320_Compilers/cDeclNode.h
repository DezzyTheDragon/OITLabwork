#pragma once
//**************************************
// cDeclNode
//
// Defines base class for all declarations.
// Future labs will add features to this class.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cSymbol.h"

class cDeclNode : public cAstNode
{
    public:
        cDeclNode() : cAstNode() {}
        virtual bool IsReal() { return false; }
        virtual bool IsInt()  { return false; }
        virtual bool IsChar() { return false; }
        virtual bool IsType() { return false; }
        virtual bool IsFunc() { return false; }
        virtual bool IsProc() { return false; }
        virtual string GetTypeName() { return "cDecl"; }
        virtual int GetTypeSize() { return 0; }
        virtual cDeclNode *GetType() { return this; }
        virtual cBlockNode *GetBlock() { return nullptr; }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        //virtual cVarDeclsNode *GetParams() { return nullptr; }
        virtual int ParamNum() { return 0; }
        virtual int GetOffset() { return m_offset; }
        virtual void SetOffset(int offset) { m_offset = offset; }
        virtual int GetSize() { return m_size; }
        virtual void SetSize(int size) { m_size = size; }
    protected:
        int m_offset = 0;
        int m_size = 0;
};

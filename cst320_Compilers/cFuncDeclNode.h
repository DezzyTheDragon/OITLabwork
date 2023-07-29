#pragma once

#include "cProcDeclNode.h"
#include "cSymbol.h"
#include "cDeclNode.h"
#include "cVarDeclsNode.h"
#include "cBlockNode.h"


class cFuncDeclNode : public cProcDeclNode
{
    public:
        cFuncDeclNode() : cProcDeclNode()
        {}
        //Name child 0
        //Type child 1
        //Parm child 2
        //Bloc child 3
        void AddFuncName(cSymbol* name)
        {
            cSymbol* localSym = g_symbolTable.LocalLookup(name->GetName());
            //if there is already a matching symbol but that symbol isn't a func type
            if(localSym != nullptr && !(localSym->GetDecl()->IsFunc()))
            {
                string errorMsg = "Symbol " + localSym->GetName() + 
                            " already exists in current scope";
                SemanticParseError(errorMsg);
            }
            if(localSym != nullptr)
            {
                name = localSym;
            }
            else    //if there is no valid local symbol then add global
            {
                cSymbol* temp = g_symbolTable.GlobalLookup(name->GetName());
                if(temp != nullptr)
                {
                    name = new cSymbol(name->GetName());
                }
            }
            AddChild(name);
            SetChild(0, name);
            if(name->GetDecl() == nullptr)
            {
                name->SetDecl(this);
            }
            g_symbolTable.Insert(name);
        }
        void AddFuncType(cDeclNode* type)
        {
            //get the symbol for this func
            //get the decl and get its type
            //compare the type
            cSymbol* sym = dynamic_cast<cSymbol*>(GetChild(0));
            //Check to make sure that the decl isn't this object
            if(sym->GetDecl() != this)
            {
                if(sym->GetDecl()->GetType() != type)
                {
                    string errorMsg = sym->GetName() + 
                            " previously declared with different return type";
                    SemanticParseError(errorMsg);
                }
            }

            AddChild(type);
            SetChild(1, type);
        }
        void AddFuncParams(cVarDeclsNode* params)
        {
            
            cSymbol* sym = dynamic_cast<cSymbol*>(GetChild(0));
            cFuncDeclNode* decl = GetFuncDecl();
            if(sym->GetDecl() != this)
            {
                //dont check the params, check the number of params
                if(decl->GetParams()->GetVarDeclCount() != params->GetVarDeclCount())
                {
                    string errorMsg = sym->GetName() + 
                            " redeclared with different number of parameters";
                    SemanticParseError(errorMsg);
                }
            }
            
            AddChild(params);
            SetChild(2, params);
        }
        void AddFuncBlock(cBlockNode* block)
        {
            cSymbol *sym = dynamic_cast<cSymbol*>(GetChild(0));
            
            cFuncDeclNode* decl = GetFuncDecl();
            if(sym->GetDecl() != this)
            {
                //if previous is not null and this is not null then
                //throw error
                if(decl->GetBlock() != nullptr && block != nullptr)
                {
                    string errorMsg = sym->GetName() + " already has a definition";
                    SemanticParseError(errorMsg);
                }
            }
            

            AddChild(block);
            SetChild(3, block);
            //set existing sym name to this one
            //cSymbol* sym = dynamic_cast<cSymbol*>(GetChild(0));
            sym->SetDecl(this);
        }
        
        cSymbol *GetName() { return dynamic_cast<cSymbol*>(GetChild(0)); }

        cVarDeclsNode *GetParams()
        {
            cVarDeclsNode *params = dynamic_cast<cVarDeclsNode*>(GetChild(2));
            return params;
        }

        virtual int ParamNum()
        {
            int count = 0;
            cVarDeclsNode *params = dynamic_cast<cVarDeclsNode*>(GetChild(2));
            if(params != nullptr)
            {
                count = params->GetVarDeclCount();
            }
            return count;
        }
        virtual cBlockNode *GetBlock()
        {
            cBlockNode *block = dynamic_cast<cBlockNode*>(GetChild(3));
            return block;
        }

        virtual bool IsFunc() { return true; }
        virtual cDeclNode *GetType()
        {
            cDeclNode *dec = dynamic_cast<cDeclNode*>(GetChild(1));
            return dec;
        }

        virtual int GetSize() { return m_size; }
        virtual void SetSize(int size) { m_size = size; }
        virtual int GetOffset() { return m_offset; }
        virtual void SetOffset(int offset) { m_offset = offset; }
        virtual string AttributesToString() 
        {
            string ret = " size=\"" + std::to_string(m_size) + "\"";
            ret += " offset=\"" + std::to_string(m_offset) + "\"";
            return ret;
        }
        virtual string NodeType() { return "func"; }
        virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
        virtual bool IsProc() { return false; }
    private:
        cFuncDeclNode *GetFuncDecl()
        {
            cSymbol *name = dynamic_cast<cSymbol*>(GetChild(0));
            return dynamic_cast<cFuncDeclNode*>(name->GetDecl()); 
            //In an announcment Phil said that this was acceptable
        }


};

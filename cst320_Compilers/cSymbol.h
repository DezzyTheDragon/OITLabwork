#pragma once
//*********************************************************
// cSymbol.h
//
// cSymbol class declaration.
// cSymbols are objects that represent identifiers and other such strings in 
// the compiler.
//
// Author: Phil Howard
// phil.howard@oit.edu
//

#include <string>
#include <utility>
#include "cAstNode.h"
#include "cDeclNode.h"
//include "pascalishparse.h"
//#include "tokens.h"

using std::string;

class cSymbol : public cAstNode
{
    public:
        cSymbol(string name, int type = 0);
        /*
        cSymbol(string name, int type = 0) : cAstNode()
        {
            m_name = name;
            if(type == 0)
            {
                //Identifier
                m_id = ++nextId;
                tokenType = IDENTIFIER;
            }
            else if(type == TYPE_ID)
            {
                //set Type
                m_id = ++nextId;
                tokenType = TYPE_ID;
            }
            else
            {
                //tokenType
                m_id = 0;
                tokenType = type;

            }
        }
        */

        // return the name of the cSymbol
        string GetName(); //{ return m_name; }
        
        int GetTokenType(); //{ return tokenType; }
        void SetTokenType(int type); //{ tokenType = type; }
        long GetId(); //{ return m_id; }
        void SetDecl(cDeclNode *node);
        cDeclNode* GetDecl();

        virtual string AttributesToString();
        /*{
            string ret(" id=\"");
            ret += std::to_string(m_id);
            ret += "\" name=\"" + m_name + "\"";
            return ret;
        }*/
        virtual string NodeType(); //{ return "sym"; }
        virtual void Visit(cVisitor *visitor); //{ visitor->Visit(this); }
    protected:
        static long long nextId;    // used to generate unique IDs
        long m_id;             // ID for this cSymbol
        string m_name;         // name for this cSymbol
        int tokenType;
        cDeclNode *m_decl;
};

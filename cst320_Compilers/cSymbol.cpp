#include "cSymbol.h"
#include "astnodes.h"
#include "pascalishparse.h"

cSymbol::cSymbol(string name, int type) : cAstNode()
{
    m_name = name;
    if(type == 0)
    {
        m_id = ++nextId;
        tokenType = IDENTIFIER;
    }
    else if(type == TYPE_ID)
    {
        m_id = ++nextId;
        tokenType = TYPE_ID;
    }
    else
    {
        m_id = 0;
        tokenType = type;
    }
}

string cSymbol::GetName()
{
    return m_name;
}

int cSymbol::GetTokenType()
{
    return tokenType;
}

void cSymbol::SetTokenType(int type)
{
    tokenType = type;
}

long cSymbol::GetId()
{
    return m_id;
}

void cSymbol::SetDecl(cDeclNode *node)
{
    m_decl = node;    
}

cDeclNode* cSymbol::GetDecl()
{
    return m_decl;
}

string cSymbol::AttributesToString()
{
    string ret(" id=\"");
    ret += std::to_string(m_id);
    ret += "\" name=\"" + m_name + "\"";
    return ret;
}

string cSymbol::NodeType()
{
    return "sym";
}

void cSymbol::Visit(cVisitor *visitor)
{
    visitor->Visit(this);
}

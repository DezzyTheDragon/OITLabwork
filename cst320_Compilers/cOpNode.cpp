//#pragma once
//**************************************
// cOpNode.h
//
// Class for an operator (e.g. + - etc.)
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "astnodes.h"
#include "pascalishparse.h"
#include "cOpNode.h"

string cOpNode::AttributesToString()   
{ 
    string result(" value=\"");

    switch (m_op)
    {
        case OR:
            result += "OR";
            break;
        case AND:
            result += "AND";
            break;
        case MOD:
            result += "MOD";
            break;
        case DIV:
            result += "DIV";
            break;
        case NOT:
            result += "NOT";
            break;
        case LE:
            result += "LE";
            break;
        case GE:
            result += "GE";
            break;
        case NOT_EQUAL:
            result += "NE";
            break;
        case '<':
            result += "LT";
            break;
        case '>':
            result += "GT";
            break;
        default:
            result += (char)m_op;
            break;
    }

    result += "\"";

    return result;
}

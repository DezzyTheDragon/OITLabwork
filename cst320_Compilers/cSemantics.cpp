#include "cSemantics.h"
#include "cFuncExprNode.h"
#include "astnodes.h"

cSemantics::cSemantics() : cVisitor()
{}

void cSemantics::VisitAllNodes(cAstNode *node)
{
    node->Visit(this);
}

void cSemantics::Visit(cFuncExprNode * node)
{
    node->VisitAllChildren(this);
    //function call must be defined
    //<func> is not a function
    //<func> is not declared
    //<func> is not fully defined
    //Check if declared
    //bool isAFunc = true;
    string errorMsg = node->GetName()->GetName();
    if(node->GetName()->GetDecl() != nullptr)
    {
        //check if function
        if(node->GetName()->GetDecl()->IsFunc() == true)
        {
            //check if there is a block
            if(node->GetName()->GetDecl()->GetBlock() == nullptr)
            {
                errorMsg += " is not fully defined";
                node->SemanticError(errorMsg);
            }

            //check for correct number of arguments 
            //if(node->GetName()->GetDecl()->GetParams()->GetVarDeclCount() 
            //        != node->GetExprs()->GetExprCount())
            if(node->GetName()->GetDecl()->ParamNum() != node->ExprsNum())
            {
                errorMsg += " called with wrong number of arguments";
                node->SemanticError(errorMsg);
            }
        }
        else
        {
            //isAFunc = false;
            errorMsg += " is not a function";
            node->SemanticError(errorMsg);
        }

        
    }
    else
    {
        //isAFunc = false;
        errorMsg += " is not declared";
        node->SemanticError(errorMsg);
    }
}

void cSemantics::Visit(cVarExprNode *node)
{
    node->VisitAllChildren(this);
    string errorMsg = "Variable " + node->GetName()->GetName();
    //Check to make sure that a variable is defined or is actually a variable
    if(node->GetName()->GetDecl() != nullptr)
    {
        //Check to make sure that it is not some other definition
        if(node->GetName()->GetDecl()->IsProc())
        {
            errorMsg += " is not defined";
            node->SemanticError(errorMsg);
        }
    }
    else
    {
        errorMsg += " is not defined";
        node->SemanticError(errorMsg);
    }
}


void cSemantics::Visit(cProcCallNode *node)
{
    node->VisitAllChildren(this);
    string errorMsg = node->GetName()->GetName();
    if(!node->GetType()->IsProc())
    {
        errorMsg += " is not a procedure";
        node->SemanticError(errorMsg);
    }
}


void cSemantics::Visit(cAssignNode *node)
{
    node->VisitAllChildren(this);
    string errorMsg = "Cannot assign ";
    //Make sure that we are working with valid nodes
    if(node->GetLeft()->GetName()->GetDecl() != nullptr 
            && node->GetRight()->GetType() != nullptr)
    {
        if(!node->GetLeft()->GetName()->GetDecl()->IsFunc()
            && !node->GetRight()->GetType()->IsFunc()
            && !node->GetLeft()->GetName()->GetDecl()->IsProc()
            && !node->GetRight()->GetType()->IsProc())
        {
            //can't assign real to non-real
            if(node->GetLeft()->GetType()->IsInt() 
                && node->GetRight()->GetType()->IsReal())
            {
                errorMsg += node->GetRight()->GetType()->GetTypeName() + " to ";
                errorMsg += node->GetLeft()->GetType()->GetTypeName();
                node->SemanticError(errorMsg);
            }
            else if(!(node->GetLeft()->GetType()->IsReal() && node->GetRight()->GetType()->IsInt()))
            {
                if(node->GetLeft()->GetType()->GetTypeSize() 
                        < node->GetRight()->GetType()->GetTypeSize())
                {
                    errorMsg += node->GetRight()->GetType()->GetTypeName() + " to ";
                    errorMsg += node->GetLeft()->GetType()->GetTypeName();
                    node->SemanticError(errorMsg);
                }
            }
        }
    }
}

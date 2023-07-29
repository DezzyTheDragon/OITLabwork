#pragma once
//**************************************
// astnodes.h
//
// Single place that includes all the ast nodes.
// Useful for lang.y and lang.l
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cProgramNode.h"
#include "cBlockNode.h"
#include "cDeclNode.h"
#include "cDeclsNode.h"
#include "cStmtNode.h"
#include "cStmtsNode.h"
#include "cExprNode.h"
#include "cIntExprNode.h"
#include "cNilNode.h"
#include "cExprsNode.h"
#include "cWriteNode.h"
#include "cRealExprNode.h"
#include "cBaseTypeNode.h"
#include "cIdListNode.h"
#include "cVarDeclsNode.h"
#include "cVarDeclNode.h"
#include "cVarExprNode.h"
#include "cAssignNode.h"
#include "cUnaryExprNode.h"
#include "cIfNode.h"
#include "cWhileNode.h"
#include "cProcDeclNode.h"
#include "cProcCallNode.h"
#include "cConstDeclNode.h"
#include "cFuncDeclNode.h"
#include "cFuncExprNode.h"
//#include "cSemantics.h"
//Banish these to the bottom for stupid circulir include reasons
#include "cOpNode.h"
#include "cBinaryExprNode.h"

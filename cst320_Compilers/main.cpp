//**************************************
// main.cpp
//
// Main function for pascal compiler
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "cSymbolTable.h"
#include "lex.h"
#include "astnodes.h"
#include "pascalishparse.h"
#include "cSemantics.h"
#include "cComputeSize.h"
#include "cCodeGen.h"
#include "initSymbolTable.h"

// define global variables
cSymbolTable g_symbolTable;
long long cSymbol::nextId;

// takes two string args: input_file, and output_file
int main(int argc, char **argv)
{
    std::cout << "Destiny Dahlgren" << std::endl;

    const char *outfile_name;
    int result = 0;

    if (argc > 1)
    {
        yyin = fopen(argv[1], "r");
        if (yyin == nullptr)
        {
            std::cerr << "ERROR: Unable to open file " << argv[1] << "\n";
            exit(-1);
        }
    }

    if (argc > 2)
    {
        outfile_name = argv[2];
    } else {
        //outfile_name = "/dev/tty";
        outfile_name = "pascalishout.sl";
    }

    //g_SymbolTable.InitRootTable();
    initSymbolTable initTable;
    initTable.InitSymbolTable(&g_symbolTable);

    result = yyparse();
    if (yyast_root != nullptr && result == 0 && yynerrs == 0)
    {
        cSemantics *semantics = new cSemantics();
        semantics->VisitAllNodes(yyast_root);

        if (yynerrs == 0)
        {
            cComputeSize sizer;
            sizer.VisitAllNodes(yyast_root);

            cCodeGen coder(outfile_name);
            coder.VisitAllNodes(yyast_root);

            //std::cout << yyast_root->ToString() << std::endl;
        }
    } 

    if (yyast_root == nullptr || result != 0 || yynerrs != 0)
    {
        std::cout << yynerrs << " Errors in compile\n";
    }

    if (result == 0 && yylex() != 0)
    {
        std::cout << "Junk at end of program\n";
    }

    return result;
}

#pragma once
//**************************************
// cSymbolTable.h
//
// Declares a nested symbol table.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
//

#include <string>
#include <map>
#include <vector>

#include "cSymbol.h"


class cSymbolTable
{
    public:
        // Type for a single symbol table. The full symbol table must logically
        // represent a stack of these.
        //
        // NOTE: You must change this declaration. You could use a typedef
        // or a class definition as appropriate
        typedef std::map<string, cSymbol*> symbolTable_t;

        // Create an empty symbol table
        cSymbolTable()
        {
            //m_table = new symbolTable_t();
            symbolTable_t temp;
            m_table.push_back(temp);
        }

        // Increase the scope: add a level to the nested symbol table
        // Return value is the newly created scope
        symbolTable_t *IncreaseScope()
        {
            symbolTable_t *temp = new symbolTable_t;
            m_table.push_back(*temp);

            return temp;
        }

        // Decrease the scope: remove the outer-most scope.
        // Returned value is the outer-most scope AFTER the pop.
        //
        // NOTE: do NOT clean up memory after poping the table. Parts of the
        // AST will probably contain pointers to symbols in the popped table.
        symbolTable_t *DecreaseScope()
        {
            m_table.pop_back();
            return &m_table.back();
        }

        // insert a symbol into the table
        // Assumes the symbol is not already in the table
        void Insert(cSymbol *sym)
        {
            if(LocalLookup(sym->GetName()) == nullptr)
            {
                m_table.back().insert(std::pair<string, cSymbol*>(sym->GetName(), sym));
            }
        }

        // Do a lookup in the nested table. 
        // Return the symbol for the outer-most match. 
        // Returns nullptr if no match is found.
        cSymbol *GlobalLookup(string name)
        {
            cSymbol* ret = nullptr;
            std::vector<symbolTable_t>::reverse_iterator r = m_table.rbegin();
            for(; r != m_table.rend(); ++r)
            {
                if(r->find(name) != r->end())
                {
                    ret = r->find(name)->second;
                    break;
                }
            }
            return ret;
        }

        // Find a symbol in the outer-most scope.
        // Does NOT search inner scopes.
        // Returns nullptr if the symbol is not found.
        cSymbol *LocalLookup(string name)
        {
            cSymbol* ret = nullptr;
            symbolTable_t::iterator it;
            it = m_table.back().find(name);
            if(it != m_table.back().end())
            {
                ret = m_table.back().find(name)->second;
            }
            return ret;
        }

    protected:
        //symbolTable_t *m_table;
        std::vector<symbolTable_t> m_table;

};

extern cSymbolTable g_symbolTable;

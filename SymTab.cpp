//
//		Implementation of the symbol table class.
//      This file has an implementation of the three functions: AddSymbol, DisplaySymbolTable, and LookupSymbol
//
#include "stdafx.h"
#include "SymTab.h"


void SymbolTable::AddSymbol( string &a_symbol, int a_loc )
{
    // If the symbol is already in the symbol table, record it as multiply defined.
	map<string, int>::iterator st = m_symbolTable.find( a_symbol );
    if( st != m_symbolTable.end() ) {

        st->second = multiplyDefinedSymbol;
        return;
    }
    // Record a the  location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}

void SymbolTable::DisplaySymbolTable() {
    cout << "Symbol Table:" << endl;
    cout << setw(10) << "Symbol #"
        << setw(15) << "Symbol"
        << setw(15) << "Location" << endl;
    int i = 0;
    for (auto& entry : m_symbolTable) {
        cout << setw(10) << i++
            << setw(15) << entry.first
            << setw(15) << entry.second << endl;
    }
    cout << endl;
}
bool SymbolTable::LookupSymbol(string& a_symbol, int& a_loc) {
    map<string, int>::iterator st = m_symbolTable.find(a_symbol);
    if (st == m_symbolTable.end()) {
        return false;
    }
    a_loc = st->second;
    return true;
};

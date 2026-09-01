//
//		Symbol table class.
//
#pragma once


// This class is our symbol table.
class SymbolTable {

public:
    
    const int multiplyDefinedSymbol = -999;

    /// <summary>
	/// Adds a symbol and its location to the symbol table.  
    /// If the symbol is already in the symbol table, 
    /// then the location of the symbol is set to multiplyDefinedSymbol to indicate that it is multiply defined.
    /// </summary>
    /// <param name="a_symbol"></param>
    /// <param name="a_loc"></param>
    void AddSymbol( string &a_symbol, int a_loc );

    /// <summary>
	/// Display the symbols in the symbol table.  The symbols are displayed in alphabetical order along with their location.
    /// </summary>
    void DisplaySymbolTable();

    /// <summary>
	/// Searches for a symbol in the symbol table.  
    /// If the symbol is found, then true is returned and the location of the symbol is returned in a_loc.
    /// </summary>
    /// <param name="a_symbol"></param>
    /// <param name="a_loc"></param>
    /// <returns></returns>
    bool LookupSymbol(string& a_symbol, int& a_loc);
private:

    // This is the actual symbol table.  The symbol is the key to the map.
    map<string, int> m_symbolTable;

};

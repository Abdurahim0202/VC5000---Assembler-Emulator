//
//		Assembler class.  This is a container for all the components
//		that make up the assembler.
//
#pragma once 

#include "SymTab.h"
#include "Instruction.h"
#include "FileAccess.h"
#include "Emulator.h"


class Assembler {

public:
    Assembler(int argc, char* argv[]);
    ~Assembler();

    /// <summary>
    /// Establishes the location of each instruction and assembler directive in the source code.
    /// </summary>
    void PassI();

    /// <summary>
    /// Generates the machine code translation of the source program.
    /// </summary>
    void PassII();

    /// <summary>
    /// Display the symbols in the symbol table.
    /// </summary>
    void DisplaySymbolTable() { m_symtab.DisplaySymbolTable(); }

    /// <summary>
    /// Run the emulator on the translation.
    /// </summary>
    void RunProgramInEmulator() { m_emul.runProgram(); 
    cout << endl << "*** End of the program ***" << endl;
    }

private:

    FileAccess m_facc;	    // File Access object
    SymbolTable m_symtab;	// Symbol table object
    Instruction m_inst;	    // Instruction object
    Emulator m_emul;        // Emulator object
};

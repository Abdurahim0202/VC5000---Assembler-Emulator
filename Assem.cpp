/*
 * Assembler main program.
 */
#include "stdafx.h"  
#include "Assembler.h"

int main( int argc, char *argv[] )
{
    Assembler assem( argc, argv );

    // Establish the location of the labels:
    assem.PassI( );

    // Display the symbol table.
    assem.DisplaySymbolTable();

    // Output the symbol table and the translation.
    assem.PassII();
    
    // Run the emulator on the Quack3200 program that was generated in Pass II.
    assem.RunProgramInEmulator();

    return 0;
}

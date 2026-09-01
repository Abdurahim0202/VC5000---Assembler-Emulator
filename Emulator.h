//
//		Emulator class - supports the emulation of Duck2200 programs
//      This file has implementations of the functions to run the program and manage the memory
//
#ifndef _EMULATOR_H      
#define _EMULATOR_H

#include "stdafx.h"
using namespace std;
class Emulator {

public:

    const static int MEMSZ = 100'000;	// The size of the memory of the Duck2200.

	// Constructor for the emulator.  Initializes the memory to all zeros.
    Emulator() {

         m_memory.resize(MEMSZ, 0);
    }
    
    /// <summary>
	/// Inserts a value into the simulated memory of the Duck2200.  The location must be between 0 and 99999.
    /// </summary>
    /// <param name="a_location"></param>
    /// <param name="a_contents"></param>
    /// <returns></returns>
    bool insertMemory(int a_location, int a_contents) {
        if (a_location < 0 || a_location >= MEMSZ) {
            return false; 
        }
        else {
            m_memory[a_location] = a_contents;
            return true; 
        }
        
    }
    
    /// <summary>
	/// Called to run a program that has been loaded into the simulated memory. 
    /// The program is executed according to the instruction set of the Duck2200. 
    /// If there is an illegal op code or a reference to memory outside of the of range 0 to 999999, 
    /// then execution is terminated with an error message.
    /// </summary>
    /// <returns></returns>
    bool runProgram() {
        int location = 100; 
        long long accumulatorValue = 0; 
        while (true) {
            if (location > 99999 || location < 0) {
				cout << "Error: instruction location out of range: " << location << endl;
                return false;
            }
            int inst = m_memory[location]; 
            int opCode = inst / 100'000; 
            int operand = inst % 100'000; 
            if (operand < 0 || operand >= MEMSZ) {
                cout << "Error: memory address out of range: " << operand
                    << " at location: " << location << endl;
                return false;
            }
            switch (opCode) {
            case 1: // add
                accumulatorValue += m_memory[operand];
                location++;
                continue;
			case 2: // Sub
				accumulatorValue -= m_memory[operand];
				location++;
				continue;
			case 3: // Mult
				accumulatorValue *= m_memory[operand];
				location++;
                continue;
            case 4: // Div
                if (m_memory[operand] == 0) {
                    cout << "Error: division by zero at location: " << location << endl;
                    return false;
                }
				accumulatorValue /= m_memory[operand];
				location++;     
				continue;
			case 5: // Load
				accumulatorValue = m_memory[operand];   
				location++;
                continue;
			case 6: // Store
				m_memory[operand] = accumulatorValue;
                location++; 
                continue;
			case 7: // Read
				cout << "Input: ";  
				cin >> m_memory[operand];
                location++; 
                continue; 
            case 8: // Write
				cout << m_memory[operand] << endl;
                location++; 
				continue; 
            case 9:  // B - unconditional branch
                location = operand;
                break;

            case 10: // BM - branch if accumulator < 0
                if (accumulatorValue < 0) location = operand;
                else location++;
                break;

            case 11: // BZ - branch if accumulator == 0
                if (accumulatorValue == 0) location = operand;
                else location++;
                break;

            case 12: // BP - branch if accumulator > 0
                if (accumulatorValue > 0) location = operand;
                else location++;
                break;
            case 13: // Halt
                return true; 
            default: 
				cout << "Illegal op code: " << opCode << endl;
				return false;
            }
        }
    }

private:

    vector<int> m_memory;  	// Memory for the Duck2200
};

#endif


//
// Assembler class.  This is a container for all the components that make up the assembler.
// 
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"
vector<string> Errors::m_ErrorMsgs;
bool Errors::m_WasError = false;

// Constructor for the assembler.
Assembler::Assembler( int argc, char *argv[] )
	: m_facc(argc, argv) {}
// Destructor for the Assembler class
Assembler::~Assembler( ) {}

/// <summary>
/// Pass I establishes the location of each instruction and assembler directive in the source code.
/// </summary>
void Assembler::PassI( ) 
{
    int currentLocation = 0;        // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    while( true ) {

        // Read the next line from the source file.
        string line; 
        if( ! m_facc.GetNextLine( line ) ) {
			return;
        }
        // Parse the line and get the instruction type.
        Instruction::InstructionType st =  m_inst.ParseInstruction( line, false );

        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement.
        if( st == Instruction::ST_End ) return;

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip other instruction types.
        if( st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr ) 
        {
        	continue;
		}
        // If the instruction has a label, record it and its location in the
        // symbol table.
        if( m_inst.isLabel( ) ) {

            m_symtab.AddSymbol( m_inst.GetLabel( ), currentLocation);
        }
        // Compute the location of the next instruction.
        currentLocation = m_inst.LocationNextInstruction(currentLocation);
    }
}

/// <summary>
/// Pass II generates the machine code translation of the source program.  
/// It also outputs the location and contents of each instruction in the translation along with the original statement.
///  If there are any errors in programm, error messages are generated and the translation is still produced for the statements that are error free.
/// </summary>

void Assembler::PassII() {
    cout << "Translation of the Programm" << endl << endl;
    cout << "Location"; 
    cout << setw(10) << "Contents"; 
    cout << setw(20) << "Original Statement" << endl << endl;
    m_facc.rewind(); 
    int currentLocation = 0;
    while (true) {
        if (currentLocation >= 99999) {
            Errors::RecordError("Insufficient memory at location: " + to_string(currentLocation));
			cout << "Insufficient Memory at location: " << currentLocation << endl;
            break;
        }
        string line;
        if (!m_facc.GetNextLine(line)) {
            Errors::RecordError("No END statement found.");
            break;
        }
        Instruction::InstructionType type = m_inst.ParseInstruction(line, true);

        string opCode = m_inst.GetOpCode();
        transform(opCode.begin(), opCode.end(), opCode.begin(), ::toupper);
        
        if (type == Instruction::ST_Comment) {
			cout << setw(35) << line << endl; 
        }
        else if (type == Instruction::ST_End) {
			cout << setw(20) << line << endl;
			cout << endl <<  "End of The Program" << endl << endl;
            string extraLines; 
            while (m_facc.GetNextLine(extraLines)) {
                Instruction::InstructionType extraType = m_inst.ParseInstruction(extraLines, false);
                if (extraType != Instruction::ST_Comment) {
                    Errors::RecordError("Statement after END statement: " + extraLines);
					cout << "Statement after END statement: " << extraLines << endl;
                    break;
                }
            }
            break; 
        }
        else if (type == Instruction::ST_AssemblerInstr) {
            if (m_inst.GetOperand().empty()) {
                Errors::RecordError("Missing operand: " + line);
				cout << "Missing operand: " << line << endl;
            }
            else {
                if (opCode == "ORG") {
                    cout << setw(20) << line << endl;
                    currentLocation = stoi(m_inst.GetOperand());
                }
                else if (opCode == "DC") {
                    int constantValue = stoi(m_inst.GetOperand());
                    cout << currentLocation << "        " << setfill('0') << setw(7) << constantValue << setfill(' ') << "\t" << line << endl;
                    if (constantValue > 99'99'999) {
                        Errors::RecordError("Constant too large: " + to_string(constantValue));
                        cout << "Constant too large: " << constantValue << endl;
                    }
                    m_emul.insertMemory(currentLocation, constantValue);
                    currentLocation++;
                }
                else if (opCode == "DS") {
                    cout << currentLocation << setw(38) << line << endl;
                    currentLocation += stoi(m_inst.GetOperand());
                }
            }
        }
        else if (type == Instruction::ST_MachineLanguage) {
            int operandLoc = 0;
			bool symbolFound = m_symtab.LookupSymbol(m_inst.GetOperand(), operandLoc);
            int machineCode = m_inst.GetNumericOpCode() * 100000 + operandLoc;
            if (!m_inst.GetOperand().empty() && !m_inst.isValidSymbol(m_inst.GetOperand())) {
                Errors::RecordError("Invalid operand: " + m_inst.GetOperand());
                cout << "Invalid Operand: " << line << endl;
                continue;
            }
            cout << currentLocation << setw(8) << "     " << setfill('0') << setw(7) << machineCode << setfill(' ') << "\t" << line << endl;
            if (!m_inst.GetOperand().empty()) {
                if (!symbolFound) {
                    Errors::RecordError("Undefined symbol: " + m_inst.GetOperand());
                    cout << "Undefined Symbol: " << m_inst.GetOperand() << endl;
                }
            }
            if (m_inst.GetNumericOpCode() == 13 && !m_inst.GetOperand().empty()) {
                Errors::RecordError("Extra Operand " + line);
                cout << "Extra Operand: " << line << endl;
            }
            else if (m_inst.GetNumericOpCode() != 13 && m_inst.GetOperand().empty()) {
                Errors::RecordError("Missing operand for OP code: " + m_inst.GetOpCode());
                cout << "Missing Operand for OP code: " << m_inst.GetOpCode() << endl;
            }
            if (operandLoc == m_symtab.multiplyDefinedSymbol) {
                Errors::RecordError("Multiply defined symbol: " + m_inst.GetOperand());
                cout << "Multiply Defined Symbol: " << m_inst.GetOperand() << endl;
            }
            if (m_inst.GetNumericOpCode() == -1) {
                Errors::RecordError("Invalid opcode: " + m_inst.GetOpCode());
                cout << "Invalid OP code: " << m_inst.GetOpCode() << endl;
            }
            m_emul.insertMemory(currentLocation, machineCode);
            currentLocation++;
        }
    }
	Errors::DisplayErrors();
}

//
// Class to parse and provide information about instructions.  Note: you will be adding more functionality.
//
#pragma once
#include "stdafx.h"
#include "Errors.h"

class Instruction {

public:

    Instruction( ) { };
    ~Instruction( ) { };

    enum InstructionType {
        ST_MachineLanguage, 		// A machine language instruction.
        ST_AssemblerInstr,  		// Assembler Language instruction.
        ST_Comment,          		// Comment or blank line
        ST_End                   	// end instruction.
    };


    /// <summary>
	/// Called to parse a line of source code and determine its type.  
    /// The line is also parsed into its label, opcode, and operand.
    /// </summary>
    /// <param name="a_buff"></param>
    /// <param name="errorReport"></param>
    /// <returns></returns>

    InstructionType ParseInstruction(string a_buff, bool errorReport) {
        m_instruction = a_buff;  // save original line before we modify it
        RemoveComment(a_buff);
        if (!parseline(a_buff, m_Label, m_OpCode, m_Operand) && errorReport) {
            Errors::RecordError("Extra element in statement: " + a_buff);
        }
        if (errorReport) {
            if (!m_Label.empty() && !isValidSymbol(m_Label)) {
                Errors::RecordError("Invalid label: " + m_Label);
            }
        }
        static map<string, int> opcodes = {
            {"ADD", 1},
            {"SUB", 2},
            {"MULT", 3},
            {"DIV", 4},
            {"LOAD", 5},
            {"STORE", 6},
            {"READ", 7},
            {"WRITE", 8},
            {"B", 9},
            {"BM", 10},
            {"BZ", 11},
            {"BP", 12},
            {"HALT", 13},
		};
    
        if (m_OpCode.empty()) return m_type = ST_Comment;

        string op = m_OpCode;
        transform(op.begin(), op.end(), op.begin(),::toupper);
		m_NumOpCode = opcodes.find(op) != opcodes.end() ? opcodes[op] : -1;

        if (op == "END") return m_type = ST_End;

        if (op == "ORG" || op == "DC" || op == "DS") return m_type = ST_AssemblerInstr;

        return m_type = ST_MachineLanguage;
    }

	// / Getter for the Op Code from the instruction. 
    inline string GetOpCode() { return m_OpCode; }

	// / Getter for the operand from the instruction.
    inline string& GetOperand() { return m_Operand; }

	// / Getter for the numeric value of the Op Code. 
    inline int GetNumericOpCode() { return m_NumOpCode; }


    /// <summary>
	/// Locates the next instruction in memory.  This function is used to determine the location of labels.
    /// </summary>
    /// <param name="a_loc"></param>
    /// <returns></returns>
    int LocationNextInstruction(int a_loc) {
        string op = m_OpCode;
        transform(op.begin(), op.end(), op.begin(), ::toupper);

        if (op == "ORG") return stoi(m_Operand);
        if (op == "DS")  return a_loc + stoi(m_Operand);
        return a_loc + 1;
    }

	// / Getter for the label of the intruction. 
    inline string& GetLabel() { return m_Label; };
    
	// / Called to determine if an instruction has a label. 
    inline bool isLabel( ) { return ! m_Label.empty(); };

    /// <summary>
	/// Called to determine if a symbol is valid.  A valid symbol must be 1-15 characters long, 
    /// start with a letter, and contain only letters and digits.
    /// </summary>
    /// <param name="str"></param>
    /// <returns></returns>

    bool isValidSymbol(const string& str) {
        if (str.empty()) return false;
        if (str.length() > 15) return false;
        if (!isalpha(str[0])) return false;
        for (int i = 1; i < str.length(); i++) {
            if (!isalnum(str[i])) return false;
        }
        return true;
    }

    
private:

    // The elemements of a instruction
    string m_Label;         // The label.
    string m_OpCode;        // The symbolic op code.
    string m_Operand;      // The first operand.

    string m_instruction;   // The original instruction.

    // Derived values.
    int m_NumOpCode;        // The numerical value of the op code.  Only applicable for machine language instructions.
    InstructionType m_type; // The type of instruction.


    /// <summary>
	/// Called to remove comments from a line.  A comment starts with a ';' and continues to the end of the line.  
    /// If there is no ';' in the line, then there is no comment and the line is unchanged.
    /// </summary>
    /// <param name="line"></param>

    void RemoveComment(string& line) {
        size_t pos = line.find(';');
        if (pos == string::npos)
        {
            return;
        }
        line.erase(pos);
    }

    /// <summary>
	/// Called to parse a line into its label, opcode, and operand.  
    /// The line should not have any comments. 
    /// If the line is blank, then all three are set to blank.  
    /// If there are extra elements in the line, then the first three are set and the function returns false.  
    /// Otherwise, it returns true.
    /// </summary>
    /// <param name="line"></param>
    /// <param name="label"></param>
    /// <param name="opcode"></param>
    /// <param name="operand"></param>
    /// <returns></returns>

    bool parseline(const string& line, string& label, string& opcode, string& operand)
    {
        istringstream ins(line);
        label = opcode = operand = "";
        if (line.empty()) return true;

        string extra;

        if (line[0] != ' ' && line[0] != '\t')
        {
            ins >> label;
        }
        ins >> opcode >> operand >> extra;

        return extra == "";
    }
};



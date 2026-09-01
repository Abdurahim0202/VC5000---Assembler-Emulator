//
// Class to manage error reporting. This file has two functions: RecordError and DisplayErrors 
// First function saves the errors in the vector, second one displays all the errors. 
//
#ifndef _ERRORS_H
#define _ERRORS_H

#include "stdafx.h"

class Errors {

public:

    /// <summary>
	/// Records an error message.  
    /// The message is stored for later display.  
    /// </summary>
    /// <param name="a_emsg"></param>
    static void RecordError(string a_emsg) {
        m_ErrorMsgs.push_back(a_emsg);
		m_WasError = true;
    }

    /// <summary>
	/// Displays all error messages that have been recorded.  
    /// If no errors were recorded, then a message indicating that is displayed.
    /// </summary>
    static void DisplayErrors() {
        if (m_WasError) {
            int counter = 1;
            cout << endl << "The following errors were found:" << endl << endl;
            for (const auto& msg : m_ErrorMsgs) {
                cout << counter << ". " << msg << endl;
                counter++;
            }
        } else {
            cout << "*** No errors were found. ***" << endl << endl;
		}
        m_WasError = false; 
        m_ErrorMsgs.clear();
    }

private:

	static vector<string> m_ErrorMsgs; // Vector to store error messages from the programm.
	static bool m_WasError; // Flag to indicate if there were any errors in the program.
};
#endif
//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"

/// <summary>
/// Accesses the input file.  The constructor opens the file and the destructor closes it.  
/// The GetNextLine function returns the next line from the file and the rewind function puts the file pointer back to the beginning of the file.
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
FileAccess::FileAccess( int argc, char *argv[] )
{
    // Check that there is exactly one run time parameter.
    if( argc != 2 ) {
        cerr << "Usage: Assem <FileName>" << endl;
        exit( 1 );
    }
    // Open the file.
    m_sfile.open( argv[1], ios::in );

    // If the open failed, report the error and terminate.
    if( ! m_sfile ) {
        cerr << "Source file could not be opened, assembler terminated."
            << endl;
        exit( 1 ); 
    }
}

// Destructor for the file access class.  Closes the file.
FileAccess::~FileAccess( )
{
    m_sfile.close( );
}


/// <summary>
/// Gets the next line from the user's Assembler file
/// </summary>
/// <param name="a_buff">Used to return the next line</param>
/// <returns>True if there is the next line</returns>
bool FileAccess::GetNextLine( string &a_buff )
{
    // If there is no more data, return false.
    if( m_sfile.eof() ) {
    
        return false;
    }
    getline( m_sfile, a_buff );
    
    // Return indicating success.
    return true;
}

// Put the file pointer back to the beginning of the file.
void FileAccess::rewind( )
{
    m_sfile.clear();
    m_sfile.seekg( 0, ios::beg );
}
    

//-----------------------------------------------------------------------------
// name: hex2cpp.cpp
// desc: hexdump to cpp
//
// author: Ge Wang (gewang@cs.princeton.edu)
//   data: Summer 2004
//-----------------------------------------------------------------------------
#include <iostream>
#include <sstream>
#include <stdio.h>
using namespace std;


// one of these days i will learn perl, or awk, or sed
int main( int argc, char ** argv )
{
    if( argc < 2 )
    {
        cerr << "hex2cpp: too few arguments..." << endl;
        exit( 1 );
    }

    string line;
    int count = 0;
    cout << "// data for " << argv[1] << ".raw..." << endl;
    cout << "unsigned char " << argv[1] << "_data[] = {" << endl;
    
    while( getline( cin, line ) )
    {
        istringstream sin( line );
        string token;
        int c = 0;
        
        // get rid of offset
        sin >> token;
        cout << "    ";
        while( sin >> token )
        {
            printf( "0x%.2x,", (token[0]-'0')*64 + (token[1]-'0')*8 + (token[2]-'0') );
            count++;
            c++;
        }

        if( c )
            cout << endl;
    }
    
    cout << "0" << endl;
    cout << "}; ";
    cout << argv[1] << "_size = " << count << ";";
    cout << endl << endl;
    
    return 0;
}
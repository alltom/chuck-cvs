// filename
"a.txt" => string filename;

<<<"setting FILENAME:", filename>>>;

// set environment
std.system( "unset FILENAME" );
std.setenv( "FILENAME", filename );


// filename
"JaneHappy17.txt" => string filename;

// srate in ms
"1" => string srate;

<<<"setting FILENAME:", filename>>>;

// set environment
std.setenv( "FILENAME", filename );
std.setenv( "SRATE", srate );

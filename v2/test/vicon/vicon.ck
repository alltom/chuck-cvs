
// number of total columns
9 => int total;
0 => int c;

// filename
"JaneHappy17.txt" => string filename;

// srate in ms
"1" => string srate;

<<<"setting FILENAME:", filename>>>;
<<<"setting SRATE:", srate>>>;

// set environment
std.setenv( "FILENAME", filename );
std.setenv( "SRATE", srate );

// spork many
while( c < total )
{
    c++;
    // set column
    std.setenv( "VCR_COLUMN", std.itoa( c ) );
    // spork
    machine.add( "yay.ck" );
}

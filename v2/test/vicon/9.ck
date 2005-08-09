
// set column number
9 => int column;

// get name
std.getenv( "FILENAME" ) => string f;
if( f.length() == 0 )
{
    <<<"set environment variable FILENAME!">>>;
    me.exit();
}


// patch
sinosc s => gain g => dac;
.1 => g.gain;


// declare Vicon Column Reader
VCR vcr;

// load file and specify column
if( !vcr.load( f, column ) )
    me.exit();

// print out some info
<<< "-----------------------" >>>;
<<< "file:", f >>>;
<<< "column:", column >>>;
<<< "name:", vcr.name() >>>;
<<< "size:", vcr.size() >>>;


// good
while( vcr.more() )
{
    // yes
    std.abs(vcr.curr()) => s.freq;
    // move to next
    vcr.next();

    // this is srate
    10::ms => now;
}

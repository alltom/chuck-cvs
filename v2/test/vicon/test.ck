
// declare Vicon Column Reader
VCR vcr;

// load file and specify column
if( !vcr.load( "a.txt", 2 ) )
    me.exit();

// print out some info
<<<"name:", vcr.name()>>>;
<<<"size:", vcr.size()>>>;

// print out column
while( vcr.more() )
{
    // the current
    <<<vcr.curr()>>>;
    // move to next
    vcr.next();
}

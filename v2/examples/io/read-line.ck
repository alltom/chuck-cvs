// default file
"example.txt" => string filename;

// look at command line
if( me.args() > 0 ) me.arg(0) => filename;

// instantiate
FileIO fio;

// open a file
fio.open( filename, FileIO.READ );

// ensure it's ok
if( !fio.good() )
{
    <<< "can't open file:", filename, "for reading..." >>>;
    me.exit();
}

// loop until end
while( fio.more() )
{
    <<< fio.readLine() >>>;
}

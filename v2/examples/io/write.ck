FileIO fout;

// open for write
fout.open( "b.txt", FileIO.WRITE );

// test
if( !fout.good() )
{
    <<< "can't open file for writing...", "" >>>;
    me.exit();
}

// write some stuff
fout.write( 1 );
fout.write( 2 );
fout.write( "boo" );

// close the thing
fout.close();


MidiIn min;
MidiMsg msg;

if( !min.open( 0 ) )
    <<<"bad">>>;

while( true )
{
    while( min.recv( msg ) )
    {
        <<<msg.data3>>>;
    }

    // wait
    5::ms => now;
}


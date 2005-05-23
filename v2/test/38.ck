
MidiOut mout;
MidiMsg msg;

if( !mout.open( 0 ) )
    <<<"bad">>>;

while( true )
{
    144 => msg.data1;
    Std.rand2( 10, 80 ) => msg.data2;
    Std.rand2( 40, 100 ) => msg.data3;

    mout.send( msg );

    // wait
    100::ms => now;
}


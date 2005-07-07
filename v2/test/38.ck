// midi (fail-correct)	

MidiOut mout;
MidiMsg msg;

if( !mout.open( 1 ) )
    <<<"bad">>>;

while( true )
{
    144 => msg.data1;
    std.rand2( 10, 80 ) => msg.data2;
    std.rand2( 40, 100 ) => msg.data3;

    mout.send( msg );

    // wait
    100::ms => now;
}


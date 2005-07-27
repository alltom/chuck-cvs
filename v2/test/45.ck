// midi event (fail-correct)
sinosc s => dac;

MidiIn min;
MidiMsg msg;
MidiRW mrw;

if( !min.open( 0 ) )
    me.exit();

mrw.open( "y.txt" );

while( true )
{
    // wait on event
    min => now;

    // loop through msg
    while( min.recv( msg ) )
    {
	if( msg.data1 == 144 ) // note on
	{
            <<<"now: ", now, "msg: ", msg.data1, msg.data2, msg.data3>>>;
	    std.mtof(msg.data2) => s.freq;
	    msg.data3 / 128.0 => s.gain;
	    mrw.write( msg, now );
	}
    }
}


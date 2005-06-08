sinosc s => dac;

MidiIn min;
MidiMsg msg;
MidiMsgOut mrw;

if( !min.open( 1 ) )
    <<<"bad">>>;

mrw.open( "z.txt" );

int count;

while( true )
{
    min => now;
    while( min.recv( msg ) )
    {
	if( msg.data1 == 144 ) // note on
	{
		<<<msg.data1>>>;
		<<<msg.data2>>>;
		<<<msg.data3>>>;
		<<<now>>>;
		<<<"---">>>;
		std.mtof(msg.data2) => s.freq;
		msg.data3 / 128.0 => s.gain;
		mrw.write( msg, now );
		<<<count + 1 => count>>>;
	}
    }
    // wait
    //5::ms => now;
}


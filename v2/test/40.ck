sinosc s => DelayL z => dac;

MidiIn min;
MidiMsg msg;

2::second => z.max => z.delay;

if( !min.open( 0 ) )
    <<<"bad">>>;

while( true )
{
    while( min.recv( msg ) )
    {
        <<<msg.data3>>>;
	if( msg.data1 == 144 ) // note on
	{
		<<<msg.data1>>>;
		0.75 * std.mtof(msg.data2) => s.freq;
		msg.data3 / 128.0 => s.gain;
	}
    }

    // wait
    5::ms => now;
}


// fmjoy.ck
// program which uses the first two axes of a connected joystick to control 
// the modulation frequency and index of modulation for FM synthesis

// carrier
sinosc c => gain g => dac;
// modulator
sinosc m => blackhole;

// carrier frequency
220 => float cf;
// modulator frequency
550 => float mf => m.freq;
// index of modulation
.5 => float index;

0.0 => g.gain;

fun void hid_mod()
{
	HidIn hi;
	hi.openJoystick( 0 );
	HidMsg msg;

	while( true )
	{
		hi => now;
		while( hi.recv( msg ) )
		{
			if( msg.type == 0 )
			{
				if( msg.which == 0 )
					( 550 + ( 500 * msg.fdata ) ) => mf => m.freq;
				else if( msg.which == 1 )
					( 100 * msg.fdata ) => index;
			}
			
			else if( msg.type == 1 )
			{
				if( msg.which == 4 )
					1.0 => g.gain;
			}

			else if( msg.type == 2 )
			{
				if( msg.which == 4 )
					0.0 => g.gain;
			}
		}
	}
}

spork ~ hid_mod();

// time-loop
while( true )
{
    // modulate
    cf + (index * mf * m.last()) => c.freq;
    // advance time by 1 samp
    1::samp => now;
}



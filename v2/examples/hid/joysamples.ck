// joysamples.ck
// simple joystick test program, plays drums samples everytime you press 
// the first, second, or third button on the first joystick

sndbuf kick => dac;
sndbuf snare => dac;
sndbuf hihat => dac;

"examples/data/kick.wav" => kick.read;
0.0 => kick.gain;

"examples/data/snare.wav" => snare.read;
0.0 => snare.gain;

"examples/data/hihat.wav" => hihat.read;
0.0 => hihat.gain;

HidIn hi;
hi.openJoystick( 0 );
HidMsg msg;

while( true )
{
	hi => now;
	while( hi.recv( msg ) )
	{
		if( msg.type == 1 )
		{
			if( msg.which == 0 )
			{
				1.0 => kick.gain;		
				0 => kick.pos;
			}
			else if( msg.which == 1 )
			{
				1.0 => snare.gain;		
				0 => snare.pos;
			}
			else if( msg.which == 2 )
			{
				1.0 => hihat.gain;		
				0 => hihat.pos;
			}
		}

	}
}

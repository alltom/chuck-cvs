// name: joysamples.ck
// desc: simple joystick test program, plays drums samples everytime 
// you press the first, second, or third button on the first joystick
// author: Spencer Salazar

// which joystick
0 => int device;

sndbuf kick => dac;
sndbuf snare => dac;
sndbuf hihat => dac;

"../data/kick.wav" => kick.read;
0.0 => kick.gain;

"../data/snare.wav" => snare.read;
0.0 => snare.gain;

"../data/hihat.wav" => hihat.read;
0.0 => hihat.gain;

HidIn hi;
HidMsg msg;

// try to open
if( !hi.openJoystick( device ) ) me.exit();
<<< "joystick ready...", "" >>>;

// infinite time loop
while( true )
{
    // wait on event
    hi => now;
    // recv messages
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

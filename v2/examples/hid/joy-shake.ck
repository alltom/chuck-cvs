// STK Shakers and joystick!

// make HidIn and HidMsg
HidIn hi;
HidMsg msg;

// open joystick 0, exit on fail
if( !hi.openJoystick( 0 ) ) me.exit();

// patch
Shakers s => dac;

int which;

// infinite event loop
while( true )
{
    // wait on HidIn as event
    hi => now;

    // messages received
    while( hi.recv( msg ) )
    {
        // axis
        if( msg.type == 0 )
        {
            if( msg.which == 0 ) (msg.fdata+1) * 300 => s.freq;
            if( msg.which == 1 ) (msg.fdata+1) * 64 => s.objects;
            if( msg.which == 2 ) (msg.fdata+1) / 2.0 => s.energy;
            if( msg.which == 3 ) (msg.fdata+1) / 2.0 => s.decay;
            <<< "freq:", s.freq(), "objects:", s.objects(), "energy:", s.energy(), "decay:", s.decay(), "preset:", s.preset() >>>;
        }

        // button down
        if( msg.type == 1 )
        {
            if( msg.which == 0 ) s.noteOn( .8 );
            if( msg.which == 2 ) { which++; (which % 22) => s.preset; }
            if( msg.which == 3 ) { which--; (which % 22) => s.preset; }
            <<< "freq:", s.freq(), "objects:", s.objects(), "energy:", s.energy(), "decay:", s.decay(), "preset:", s.preset() >>>;
        }
    }
}

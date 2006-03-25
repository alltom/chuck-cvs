// name: fmjoy.ck
// desc: program which uses the first two axes of a connected joystick
// to control the modulation frequency and index of modulation for FM synthesis
// author: Spencer Salazar

// which joystick
0 => int device;

// carrier
sinosc c => gain g => dac;
// modulator
sinosc m => blackhole;

// carrier frequency
220 => float cf;
// modulator frequency
550 => float mf => m.freq;
// index of modulation
.25 => float index;
// set initial gain
0.0 => g.gain;

// hid objects
HidIn hi;
HidMsg msg;

// try
if( !hi.openJoystick( device ) ) me.exit();
<<< "joystick ready...", "" >>>;

// control function
fun void hid_mod()
{
    // infinite time loop
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

// spork it
spork ~ hid_mod();

// time-loop
while( true )
{
    // modulate
    cf + (index * mf * m.last()) => c.freq;
    // advance time by 1 samp
    1::samp => now;
}

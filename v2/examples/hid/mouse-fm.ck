// name: mouse-fm.ck
// desc: uses first X/Y axes of a mouse to control mf and index for FM
// author: Spencer Salazar

// which mouse
0 => int device;

// modulator to carrier
sinosc m => sinosc c => Envelope e => dac;

// carrier frequency
220 => c.freq;
// modulator frequency
550 => m.freq;
// index of modulation
1000 => m.gain;

// phase modulation is FM synthesis (sync is 2)
2 => c.sync;

// attack
10::ms => e.duration;
.5 => e.gain;
// variables
int base;
float a0;
float a1;
float a2;
int count;

// start things
set( base, a0, a1, a2 );

// hid objects
HidIn hi;
HidMsg msg;

// try
if( !hi.openMouse( device ) ) me.exit();
<<< "mouse ready...", "" >>>;

// infinite time loop
while( true )
{
    // wait on event
    hi => now;
    // loop over messages
    while( hi.recv( msg ) )
    {
        if( msg.is_mouse_motion() )
        {
            msg.deltaX * .001 + a0 => a0;
            //else if( msg.which == 1 ) msg.fdata => a1;
            msg.deltaY * .001 + a1 => a1;
            set( base, a0, a1, a2 );
        }

        else if( msg.is_button_down() )
        {
            msg.which => base;
            count++;
            if( count == 1 ) e.keyOn();
            set( base, a0, a1, a2 );
        }

        else if( msg.is_button_up() )
        {
            msg.which => base;
            count--;
            if( !count ) e.keyOff();
        }
    }
}

// mapping function
fun void set( int base, float v0, float v1, float v2 )
{
    // modulator frequency
    ( 500 + 5*base + ( 500 * v0) ) => m.freq;
    // carrier frequency
    ( 220 + (220 * v2) ) => c.freq;
    // index of modulation
    ( 1000 * (v1+1) ) => m.gain;
    <<< "carrier:", c.freq(), "modulator:", m.freq(), "index:", m.gain() >>>;
}

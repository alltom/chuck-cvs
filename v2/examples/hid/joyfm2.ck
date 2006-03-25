// name: fmjoy.ck
// desc: program which uses the first two axes of a connected joystick
// to control the modulation frequency and index of modulation for FM synthesis
// author: Spencer Salazar

// which joystick
0 => int device;

// carrier
sinosc c => Envelope e => dac;
// modulator
sinosc m => blackhole;

// carrier frequency
220 => float cf;
// modulator frequency
550 => float mf => m.freq;
// index of modulation
.5 => float index;

// attack
10::ms => e.duration;
.25 => e.gain;
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
                if( msg.which == 0 ) msg.fdata => a0;
                else if( msg.which == 1 ) msg.fdata => a1;
                else if( msg.which == 3 ) msg.fdata => a2;
                set( base, a0, a1, a2 );
            }

            else if( msg.type == 1 )
            {
                msg.which => base;
                count++;
                if( count == 1 ) e.keyOn();
                set( base, a0, a1, a2 );
            }

            else if( msg.type == 2 )
            {
                msg.which => base;
                count--;
                if( !count ) e.keyOff();
            }
        }
    }
}

// mapping function
fun void set( int base, float v0, float v1, float v2 )
{
    ( 500 + 5*base + ( 500 * v0) ) => mf => m.freq;
    ( 220 + (220 * v2) ) => cf => c.freq;
    ( 10 * (v1+1) ) => index;
//    <<< "carrier:", cf, "modulator:", mf, "index:", index >>>;
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

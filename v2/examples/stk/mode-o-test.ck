//---------------|
// modal demo
// by: Ge Wang (gewang@cs.princeton.edu)
//     Perry R. Cook (prc@cs.princeton.edu)
//------------------|

// our patch
ModalBar modey => JCRev r => dac;

// set the gain
.95 => r.gain;
// set the reverb mix
.1 => r.mix;

// scale
[ 0, 2, 4, 7, 9, 11 ] @=> int scale[];

int which;
// our main loop
while( true )
{
    // cycle through
    ( which + 1 ) % 9 => which;
    which => modey.preset;

    // frequency...
    scale[std.rand2(0,scale.cap()-1)] => int freq;
    math.mtof( 33 + (std.rand2(0,3)*12) + freq ) => modey.freq;

    // velocity
    std.rand2f( 0.4, 0.8 ) => float v;

    <<< "playing:", which, "freq:", std.ftom(modey.freq()) $ int, "vel:", v >>>;

    repeat( 4 )
    {
        // position
        std.rand2f( 0.2, 0.8 ) => float p => modey.strikePosition;
        // strike it!
        v => modey.strike;
        // print
        <<< "    strike position:", p >>>;
        // advance time
        250::ms => now;
    }
}


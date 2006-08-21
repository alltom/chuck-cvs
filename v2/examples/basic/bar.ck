// point of attack for random otf demo

SinOsc s => dac;
.2 => s.gain;

// an array: add stuff
[ 0 ] @=> int hi[];

while( true )
{
    // change parameters here
    std.mtof( 45 + std.rand2(0,0) * 12 +
        hi[std.rand2(0,hi.cap()-1)] ) => s.freq;

    // different rate
    200::ms => now;
}

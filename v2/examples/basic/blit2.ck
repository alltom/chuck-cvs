// patch
BlitSaw s => JCRev r => dac;
.5 => s.gain;
.05 => r.mix;

// an array
[ 0, 2, 4, 7, 9, 11 ] @=> int hi[];

// infinite time loop
while( true )
{
    // frequency
    Std.mtof( 33 + Std.rand2(0,3) * 12 +
        hi[Std.rand2(0,hi.cap()-1)] ) => s.freq;

    // harmonics
    Std.rand2( 1, 5 ) => s.harmonics;

    // advance time
    120::ms => now;
}

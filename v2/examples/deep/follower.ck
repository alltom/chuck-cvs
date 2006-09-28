// SIMPLE ENVELOPE FOLLOWER, by P. Cook

// patch
adc => Gain g => OnePole p => blackhole;
// square the input
adc => g;
// multiply
3 => g.op;

// set pole position
0.9999 => p.pole;

// loop on
while( true ) {
    // <<< p.last() >>>;
    if( p.last() > 0.1 )
        <<< "BANG!!" >>>;
    0.05 :: second => now;
}

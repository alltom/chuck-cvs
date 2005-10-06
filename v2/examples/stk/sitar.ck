// patch
Sitar sit => PRCRev r => dac;
.15 => r.mix;

// time loop
while( true )
{
    // freq
    std.rand2( 0, 11 ) => float winner;
    std.mtof( 57 + std.rand2(0,3) * 12 + winner ) => sit.freq;

    // pluck!
    std.rand2f( 0.4, 0.9 ) => sit.noteOn;

    // advance time
    if ( std.randf() > 0.0 ) {
        .5::second => now;
    } else { 
        0.25::second => now;
    }
}

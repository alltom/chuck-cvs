// the world's lamest instrument

impulse i => dac;

// infinite time-loop
while( 5::ms => now )
    while( midiin => int min )
    {
        // send an impulse
        1.0 => i.value;
        // advance time a little (by some samples)
        std.rand2(2, 50)::samp => now;
    }

// power up!
// - philipd

0.0 => float t;
noise n;

// timer shred
fun void timer() { 
    while ( true ) { 
        t + 0.01 => t;
        10::ms => now;
    }
}

// spork the timer
spork ~ timer();

// sweep shred
fun void sweep( float start, float inc, float end , int c)
{
    n => TwoPole z => dac; 
    1  => z.norm;
    0.2 => z.gain;
    t => float st;
    start => float frq;
    std.rand2f ( 0.98, 0.99 ) => z.radius;
    while ( t < end ) {
        math.max ( ( t - st ) * 4.0, 1.0 ) * 0.2  => z.gain; 
        frq + inc * 0.01  => frq; 
        frq => z.freq;
        10::ms => now;
    }
    n =< z;
    z =< dac;
}

0 => int c;
// time loop
while ( true ) { 
    125::ms => dur d;
    if ( std.rand2 ( 0, 10 ) > 3 ) d * 2.0 => d;
    if ( std.rand2 ( 0, 10 ) > 6 ) d * 3.0 => d;
    spork ~ sweep( 220.0 * (float)std.rand2(1,8), 
        880.0 + std.rand2f(100.0, 880.0), t + std.rand2f(1.0, 3.0) , c);
    1 + c => c; 
    d => now;
}


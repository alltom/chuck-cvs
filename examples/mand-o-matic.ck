//---------------|
// mand-o-matic!
// by: Ge Wang (gewang@cs.princeton.edu)
//     Perry R. Cook (prc@cs.princeton.edu)
//------------------|

// our patch
Mandolin mand => JCRev r => Echo a => Echo b => Echo c => dac;
// set the gain
.15 => r.gain;
// set the reverb mix
.2 => r.mix;
// set max delay for echo
1000::ms => a.max => b.max => c.max;
// set delay for echo
750::ms => a.delay => b.delay => c.delay;
// set the initial effect mix
0.0 => a.mix => b.mix => c.mix;

// shred to modulate the mix
fun void echo_shred( )
{
    0.0 => float decider => float mix => float old => float inc;

    // time loop
    while( true )
    {
        std.rand2f(0.0,1.0) => decider;
        if( decider < .35 ) 0.0 => mix;
        else if( decider < .55 ) .08 => mix;
        else if( decider < .8 ) .5 => mix;
        else .15 => mix;

        // find the increment
        (mix-old)/1000.0 => inc; 1000 => int n;
        // time loop
        while( n-- )
        {
            // set the mix for a, b, c
            old + inc => old => a.mix => b.mix => c.mix;
            1::ms => now;
        }
        // remember the old
        mix => old;
        // let time pass until the next iteration
        std.rand2(2,6)::second => now;
    }
}

// let echo shred go
spork ~ echo_shred();

// our main loop
while( true )
{
    // position
    std.rand2f( 0.2, 0.8 ) => mand.pluckPos;

    // frequency...
    2 * std.rand2( 0, 4 ) => int freq;
    if( freq == 6 ) 7 => freq; if( freq == 8 ) 9 => freq;
    freq + 57 + std.rand2(0,2) * 12 => freq; 
    std.mtof ((float) freq ) => mand.freq;

    // pluck it!
    std.rand2f( 0.2, 0.9 ) => mand.pluck;

    if( std.randf() > 0.8 )
    { 500::ms => now; }
    else if( std.randf() > .85 )
    { 250::ms => now; }
    else if( std.randf() > -0.9 )
    { .125::second => now; }
    else
    {
        1 => int i => int pick_dir;
        // how many times
        4 * std.rand2( 1, 5 ) => int pick;
        0.0 => float pluck;
        0.7 / (float)pick => float inc;
        // time loop
        for( ; i < pick; i++ )
        {
            75::ms => now;
            std.rand2f(.2,.3) + (float)i*inc => pluck;
            pluck + -.2 * (float)pick_dir => mand.pluck;
            // simulate pluck direction
            !pick_dir => pick_dir;
        }
        // let time pass for final pluck
        75::ms => now;
    }
}

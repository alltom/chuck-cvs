// run each stooge, or run three stooges concurrently
// %> chuck moe++ larry++ curly++

// source to filter to dac
sndbuf i => biquad f => gain g => dac;
// second formant
i => biquad f2 => g;
// third formant
i => biquad f3 => g;

// set the filter's pole radius
.995 => f.prad; .995 => f2.prad; .995 => f3.prad;
// set equal gain zeros
1 => f.eqzs; 1 => f2.eqzs; 1 => f3.eqzs;
// initialize float variable
0.0 => float v;
// set filter gain
.1 => f.gain; .1 => f2.gain; .1 => f3.gain;
// load glottal pop
"special:glot_pop" => i.read;
// play
1.0 => i.rate;

  
// infinite time-loop   
while( true )
{
    // set the current pos
    0 => i.pos;

    // sweep the filter resonant frequency
    660.0 + std.rand2f( 0.0, 10.0 ) => f.pfreq;
    1780.0 + std.rand2f( 0.0, 5.0 ) => f2.pfreq;
    2410.0 + std.rand2f( 0.0, 40.0 ) => f3.pfreq;

    // increment v
    v + .05 => v;
    // gain
    0.3 + math.sin(v)*.15 => g.gain;
    // advance time
    (80.0 + math.sin(v*2.0)*10.0)::ms => now;
}

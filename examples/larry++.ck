// run each stooge, or run three stooges concurrently
// %> chuck moe larry curly

// impulse to filter to dac
sndbuf i => biquad f => gain g => JCRev r => dac;
// second formant
i => biquad f2 => g;
// third formant
i => biquad f3 => g;

// set the filter's pole radius
.995 => f.prad; .995 => f2.prad; .995 => f3.prad;
// set equal gain zeros
1 => f.eqzs; 1 => f2.eqzs; 1 => f3.eqzs;
// initialize float variable
0.0 => float v => float v2 => float v3;
// set filter gain
.1 => f.gain; .1 => f2.gain; .1 => f3.gain;
.05 => r.mix;
// load glottal ahh
"special:glot_pop" => i.read;
// play
1.0 => i.rate;
  
// infinite time-loop   
while( true )
{
    // set the current sample/impulse
    0 => i.pos;
    // sweep the filter resonant frequency
    250.0 + math.sin(v) * 10.0 => v2;
    v2 => f.pfreq; 2290.0 + math.sin(v)*80.0 => f2.pfreq;
    3010.0 => f3.pfreq;
    // increment v
    v + .1 => v;
    // randomize gain
    std.rand2f( .02, .2 ) => g.gain;
    // advance time
    (71.0+math.sin(v)*10.0)::ms => now;
}


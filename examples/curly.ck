// run each stooge, or run three stooges concurrently
//   %> chuck moe.ck larry.ck curly.ck

// impulse to filter to dac
impulse i => biquad f => dac;
// set the filter's pole radius
.99 => f.prad; 
// set equal gain zeros
1 => f.eqzs;
// initialize float variable
0.0 => float v;
  
// infinite time-loop   
while( true )
{
    // set the current sample/impulse
    1.0 => i.value;
    // sweep the filter resonant frequency
    std.abs(math.sin(v)) * 4000.0 => f.pfreq;
    // increment v
    v + .1 => v;
    // advance time
    101::ms => now;
}


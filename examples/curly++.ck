// run each stooge, or run three stooges concurrently
// %> chuck moe++ larry++ curly++

// impulse to filter to dac
sndbuf i => JCRev r => dac;

// load glottal ooo
"special:glot_ooo" => i.read;
// play
1.0 => i.rate;
.1 => r.mix;
0.0 => float v;
  
// infinite time-loop   
while( true )
{
    // set the current sample/impulse
    0 => i.pos;
    // randomize gain
    std.rand2f( .4, 2.0 ) => r.gain;
    v + .1 => v;
    // advance time
    (69.0+math.sin(v)*10.0)::ms => now;
}


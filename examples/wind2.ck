// noise generator, biquad filter, dac (audio output) 
noise n => biquad f => dac;
// set biquad pole radius
.99 => f.prad;
// set biquad gain
.05 => f.gain;
// set equal zeros 
1 => f.eqzs;
// our float
0.0 => float t;

// concurrent control
fun void wind_gain( )
{
    0.0 => float t; 

    // time loop to ramp up the gain / oscillate
    while( true )
    {
        0.1 * std.abs( math.sin( t ) ) => n.gain;
        t + .001 => t;
        10::ms => now;
    }
}

// run wind_gain on anothre shred
spork ~ wind_gain();

// infinite time-loop
while( true )
{
    // sweep the filter resonant frequency
    100.0 + std.abs(math.sin(t)) * 1000.0 => f.pfreq;
    t + .01 => t;
    // advance time
    100::ms => now;
}

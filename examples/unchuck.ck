// noise generator, biquad filter, dac (audio output) 
noise n => biquad f => dac;
// set biquad pole radius
.99 => f.prad;
// set biquad gain
.1 => f.gain;
// set equal zeros 
1 => f.eqzs;
// our float
0.0 => float t;

3::second + now => time later;
// time-loop
while( now < later )
{
    // sweep the filter resonant frequency
    100.0 + std.abs(math.sin(t)) * 1000.0 => f.pfreq;
    t + .05 => t;
    // advance time
    100::ms => now;
}

// unlink the ugen f from dac
f =< dac;

// let more time pass
3::second => now;

// relink
f => dac;

// time-loop
3::second + now => later;
while( now < later )
{
    // resume sweep
    100.0 + std.abs(math.sin(t)) * 1000.0 => f.pfreq;
    t + .05 => t;
    // advance time
    100::ms => now;    
}

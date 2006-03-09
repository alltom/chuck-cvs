// FM synthesis by hand

// carrier
sinosc c => dac;
// modulator
sinosc m => blackhole;

// carrier frequency
220 => float cf;
// modulator frequency
550 => float mf => m.freq;
// index of modulation
.5 => float index;

// time-loop
while( true )
{
    // modulate
    cf + (index * mf * m.last()) => c.freq;
    // advance time by 1 samp
    1::samp => now;
}

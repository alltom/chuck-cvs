// actual FM using sinosc (sync is 0)

// modulator to carrier
SinOsc m => SinOsc c => dac;

// carrier frequency
220 => float cf => c.freq;
// modulator frequency
20 => m.freq;
// index of modulation
200 => m.gain;

// time-loop
while( true )
{
    1::second => now;
}

// FM using sinosc

// modulator to carrier
sinosc m => sinosc c => dac;

// carrier frequency
220 => float cf => c.freq;
// modulator frequency
550 => m.freq;
// index of modulation
200 => m.gain;
// phase modulation is FM synthesis
2 => c.sync;

// time-loop
while( true )
{
    1::second => now;
}


adc => gain g => JCRev r => dac;
sinosc s => g;

// multiply inputs
3 => g.op;

// presets
.2 => r.mix;
400.0 => s.freq;

// time loop
while( true )
    1::second => now;

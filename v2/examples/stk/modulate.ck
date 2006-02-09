// patch into gain
sinosc s => gain g => dac;
Modulate mod => g;

// multiply
3 => g.op;

// set freq
220 => s.freq;

// set rate in hz
mod.vibratoRate( 6.0 );
// set gain
mod.vibratoGain( .2 );
// set random gain
mod.randomGain( .1 );

// infinite time loop
while( true )
{
    10::second => now;
}

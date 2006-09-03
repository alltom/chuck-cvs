// our patch
Noise n => LPF lpf => dac;

// infinite time-loop
float t;
while( true )
{
    // sweep the cutoff
    100 + Std.fabs(Math.sin(t)) * 5000 => lpf.freq;
    // increment t
    .005 +=> t;
    // advance time
    5::ms => now;
}

//fm synth for the new osc..
//to be legit, we should be polling another sinosc, but that's not here yet
//so this is rough...

// - pld  06/17/04

440.0 => float fc;
100.0 => float fm;
100.0  => float ampM;
0.0 => float frq;
0.0 => float t;
sinosc s => dac;
fc => s.sfreq;
0.5 => s.gain;

//timing
while ( true ) {
    //fun sound
    120.0 + 120.0 * math.sin( t * 0.2 ) => fm;  //modulate modulators
    100.0 + 50.0 * math.sin ( t * 0.05 ) => ampM; //modulate mod amplitude.

    fc + ampM * math.sin ( fm * t * 2.0 * pi) => frq; //fm eq..
    frq => s.sfreq;
    t + 0.001 => t;

    // for fun, mess with things at 1::samp => now;
    1::ms => now; 
}

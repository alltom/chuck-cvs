//fm synth for the new osc..
//to be legit, we should be polling another sinosc, but that's not here yet
//so this is rough...

// - pld  06/17/04

440.0 => float fc;
100.0 => float fm;
100.0  => float ampM;
0.0 => float frq;
0.0 => float t;


sinosc modulation => sinosc carrier => dac;

fm => modulation.sfreq;
fc => carrier.sfreq;
0.5 => carrier.gain;

fun void fmloop() {
	while ( true ) { 
		fc + ampM * modulation.last => carrier.sfreq;
		1::samp => now;
	}
}

spork ~fmloop();

while ( true ) {

    120.0 + 120.0 * math.sin( t * 0.2 ) => fm;  //modulate modulators
    fm => modulation.sfreq;

    100.0 + 50.0 * math.sin ( t * 0.05 ) => ampM; //modulate mod amplitude.

    t + 0.001 => t;
    1::ms => now; 
}

// name: plu.ck
// desc: karplus + strong plucked string filter
// author: Ge Wang (gewang@cs.princeton.edu)

// feedforward
Impulse imp => OneZero lowpass => dac;
// feedback
lowpass => Delay delay => lowpass;

// our radius
.99999 => float R;
// our delay order
100 => float L;
// set delay
L::samp => delay.delay;
// set dissipation factor
Math.pow( R, L ) => delay.gain;
// place zero
-1 => lowpass.zero;

// fire an impulse
1.0 => imp.next;

// advance time
12::second => now;
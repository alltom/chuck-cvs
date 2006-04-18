// name: clix.ck
// desc: typing-based instrument, quantized
// author: Ge

// computer key input, with sound
KBHit kb;

// time
4096::samp => dur T;

// patch
impulse i => biquad f => Envelope e => JCRev r => dac;
// set the filter's pole radius
.99 => f.prad;
// set equal gain zeros
1 => f.eqzs;
// initialize float variable
0.0 => float v;
// set filter gain
.5 => f.gain;
// envelope rise/fall time
2::ms => e.duration;
// reverb mix
.02 => r.mix;

// strengths
[1.0, 0.2, 0.3, 0.2, 0.4, 0.1, 0.2, 0.1,
0.5, 0.1, 0.3, 0.2, 0.4, 0.1, 0.2, 0.1,
0.8, 0.1, 0.3, 0.2, 0.5, 0.1, 0.2, 0.1,
0.4, 0.1, 0.3, 0.2, 0.3, 0.1, 0.2, 0.1] @=> float mygains[];

// capacity
mygains.cap() => int N;
// total period
N * T => dur period;

// time-loop
while( true )
{
    // wait on event
    kb => now;

    // synch
    T - (now%T) => now;

    // loop through 1 or more keys
    while( kb.more() )
    {
        // figure out period
        (now % period / period * N) $ int => int index;
        // generate impulse
        mygains[index] => i.next;
        // set filtre freq
        kb.getchar() => int c => std.mtof => f.pfreq;
        // print int value
        <<< "ascii:", c, "velocity:", mygains[index] >>>;

        // open
        e.keyOn();
        // advance time
        T-2::ms => now;
        // close
        e.keyOff();
        2::ms => now;
    }
}

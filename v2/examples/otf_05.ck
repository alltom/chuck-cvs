//----------------------------|
// on-the-fly synchronization
// adapted from Perry's ChucK Drummin' + Ge's sine poops
//
// authors: Perry Cook (prc@cs.princeton.edu)
//          Ge Wang (gewang@cs.princeton.edu)
// --------------------|          
// add one by one into VM (in pretty much any order):
// 
// terminal-1%> chuck --loop
// ---
// terminal-2%> chuck + otf_01.ck
// (anytime later)
// terminal-2%> chuck + otf_02.ck
// (etc...)
//--------------------------------------|

// synchronize to period
.5::second => dur T;
T - (now % T) => now;

// connect patch
SinOsc s => JCRev r => dac;
.25 => s.gain;
.1 => r.mix;

// scale (in semitones)
[ 0, 2, 4, 7, 9 ] @=> int scale[];

// infinite time loop
while( true )
{
    // get note class
    scale[ Math.rand2(0,4) ] => float freq;
    // get the final freq    
    Std.mtof( 21.0 + (Std.rand2(3,6)*12 + freq) ) => s.freq;

    // advance time
    .25::T => now;
}

//----------------------------|
// on-the-fly synchronization
// adapted from Perry's ChucK Drummin' + Ge's sine poops
//
// authors: Perry Cook (prc@cs.princeton.edu)
//          Ge Wang (gewang@cs.princeton.edu)
//
// terminal-1%> chuck --loop
// ---
// terminal-2%> chuck + otf_01.ck
// (anytime later)
// terminal-2%> chuck + otf_02.ck
// (etc...)
//--------------------------------------|

// this synchronizes to period
500::ms => dur T;
T - (now % T) => now;

// construct the patch
sndbuf buf => dac;
"data/kick.wav" => buf.read;
.8 => buf.gain;

// time loop
while( true )
{
    0 => buf.pos;
    std.rand2f(.8,.9) => buf.gain;
    1::T => now;
}

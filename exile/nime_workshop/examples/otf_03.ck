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

// this synchronizes to period
.5::second => dur T;
T - (now % T) => now;

// extra half T offset
.5::T => now;

// construct the patch
sndbuf buf => gain g => dac;
"data/hihat-open.wav" => buf.read;
.5 => g.gain;

// time loop
while( true )
{
    0 => buf.pos;
    std.rand2f(.8,.9) => buf.gain;
    1::T => now;
}

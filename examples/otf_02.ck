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
"data/hihat.wav" => buf.read;
.125 => buf.gain;

// time loop
while( true )
{
    std.rand2f(.4,.9) => buf.gain;
    if( std.randf() > 0.75 )
    {
        0 => buf.pos;
        .5::T => now;
    }
    else
    {
        0 => buf.pos;
        .25::T => now;
        0 => buf.pos;
        .25::T => now;
    }
}

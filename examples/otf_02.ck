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
1.25::second => dur T;
T - (now % T) => now;

// construct the patch
sndbuf buf => gain g => dac;
"puzzler.au" => buf.read;
.90 => g.gain;

1.2 => buf.rate;
0.7 => buf.phase;
// time loop
while( true )
{
    std.rand2f(.4,.9) => buf.gain;
    if( std.randf() > 0.75 )
    {
        0.8 => buf.phase;
        .5::T => now;
    }
    else
    {
        0.5 => buf.phase;
        .25::T => now;
        0.8 => buf.phase;
        .25::T => now;
    }
}

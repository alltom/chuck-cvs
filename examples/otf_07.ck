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
sndbuf buf => JCRev r => dac;
"data/snare-chili.wav" => buf.read;
.25 => buf.gain;
.1 => r.mix;

// where we actually want to start
50 => int where;

// time loop
while( true )
{
    std.rand2f(.8,.9) => buf.gain;

    if( std.randf() > .5 )
    {
        0 => int i;
        while( i < 8 )
        {
            (float)i / 8.0 => buf.gain;
            where => buf.pos;
            (1.0/8.0)::T => now;
            i++;
        }

        where => buf.pos;
        .5::T => now;
        where => buf.pos;
        .5::T => now;
    }
    else
    {
//        std.rand2( 1, 3 ) => int n;

//        while( n > 0 )
        { 
            .9 => buf.gain;
            where => buf.pos;
            .25::T => now;
            .3 => buf.gain;
            where => buf.pos;
            .25::T => now;
            .3 => buf.gain;
            where => buf.pos;
            .25::T => now;

            .9 => buf.gain;
            where => buf.pos;
            .25::T => now;
            .3 => buf.gain;
            where => buf.pos;
            .25::T => now;
            .3 => buf.gain;
            where => buf.pos;
            .25::T => now;

            .9 => buf.gain;
            where => buf.pos;
            .25::T => now;
            .3 => buf.gain;
            where => buf.pos;
            .25::T => now;
        }
    }
}

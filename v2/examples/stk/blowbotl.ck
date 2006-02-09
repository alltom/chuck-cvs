// patch
BlowBotl bottle => dac;

// noise gain
bottle.controlChange( 4, 64 );
// vibrato freq
bottle.controlChange( 11, 64 );
// vibrato gain
bottle.controlChange( 1, 60 );
// volume
bottle.controlChange( 128, 72 );


// scale
[0, 2, 4, 7, 8, 11] @=> int scale[];

// infinite time loop
while( true )
{
    // set freq
    scale[std.rand2(0,scale.cap()-1)] + 57 => std.mtof => bottle.freq;
    // go
    .8 => bottle.noteOn;

    // advance time
    1::second => now;
}

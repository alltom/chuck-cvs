// patch
BlowBotl bottle => dac;

// scale
[0, 2, 4, 7, 8, 11] @=> int scale[];

// infinite time loop
while( true )
{
    // noise gain
    bottle.controlChange( 4, std.rand2f(32, 128) );
    // vibrato freq
    bottle.controlChange( 11, std.rand2f(32, 96) );
    // vibrato gain
    bottle.controlChange( 1, std.rand2f(32, 96) );
    // volume
    bottle.controlChange( 128, std.rand2f(32, 96) );

    // set freq
    scale[std.rand2(0,scale.cap()-1)] + 57 => std.mtof => bottle.freq;
    // go
    .8 => bottle.noteOn;

    // advance time
    1::second => now;
}

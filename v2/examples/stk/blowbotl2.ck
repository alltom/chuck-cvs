// STK BlowBotl; using classic STK controlChange
// (also see blowbotl.ck)

// patch
BlowBotl bottle => dac;

// scale
[0, 2, 4, 7, 8, 11] @=> int scale[];

// infinite time loop
while( true )
{
    std.rand2f( 0, 128 ) => float noisegain;
    std.rand2f( 0, 128 ) => float vibratofreq;
    std.rand2f( 0, 128 ) => float vibratogain;
    std.rand2f( 0, 128 ) => float volume;

    <<< "---", "" >>>;
    <<< "noise gain:", noisegain >>>;
    <<< "vibrato freq:", vibratofreq >>>;
    <<< "vibrato gain:", vibratogain >>>;
    <<< "volume:", volume >>>;

    // noise gain
    bottle.controlChange( 4, noisegain );
    // vibrato freq
    bottle.controlChange( 11, vibratofreq );
    // vibrato gain
    bottle.controlChange( 1, vibratogain );
    // volume
    bottle.controlChange( 128, volume );

    // set freq
    scale[std.rand2(0,scale.cap()-1)] + 57 => std.mtof => bottle.freq;
    // go
    .8 => bottle.noteOn;

    // advance time
    1::second => now;
}

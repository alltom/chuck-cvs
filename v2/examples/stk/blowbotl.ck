// STK BlowBotl

// patch
BlowBotl bottle => dac;

// scale
[0, 2, 4, 7, 8, 11] @=> int scale[];

// infinite time loop
while( true )
{
    // ding!
    std.rand2f( 0, 1 ) => bottle.noiseGain;
    std.rand2f( 0, 12 ) => bottle.vibratoFreq;
    std.rand2f( 0, 1 ) => bottle.vibratoGain;
    std.rand2f( 0, 1 ) => bottle.volume;

    // print
    <<< "---", "" >>>;
    <<< "noise gain:", bottle.noiseGain() >>>;
    <<< "vibrato freq:", bottle.vibratoFreq() >>>;
    <<< "vibrato gain:", bottle.vibratoGain() >>>;
    <<< "volume:", bottle.volume() >>>;

    // set freq
    scale[std.rand2(0,scale.cap()-1)] + 57 => std.mtof => bottle.freq;
    // go
    .8 => bottle.noteOn;

    // advance time
    1::second => now;
}

// patch
Bowed bow => dac;

// scale
[0, 2, 4, 7, 8, 11] @=> int scale[];

// infinite time loop
while( true )
{
    std.rand2f( 0, 1 ) => bow.bowPressure;
    std.rand2f( 0, 1 ) => bow.bowPosition;
    std.rand2f( 0, 12 ) => bow.vibratoFreq;
    std.rand2f( 0, 1 ) => bow.vibratoGain;
    std.rand2f( 0, 1 ) => bow.volume;

    <<< "---", "" >>>;
    <<< "bow pressure:", bow.bowPressure() >>>;
    <<< "bow position:", bow.bowPosition() >>>;
    <<< "vibrato freq:", bow.vibratoFreq() >>>;
    <<< "vibrato gain:", bow.vibratoGain() >>>;
    <<< "volume:", bow.volume() >>>;

    // set freq
    scale[std.rand2(0,scale.cap()-1)] + 57 => std.mtof => bow.freq;
    // go
    .8 => bow.noteOn;

    // advance time
    std.rand2f(.8, 2)::second => now;
}

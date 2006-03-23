// STK ModalBar

// patch
ModalBar bar => dac;

// scale
[0, 2, 4, 7, 8, 11] @=> int scale[];

// infinite time loop
while( true )
{
    // ding!
    std.rand2( 0, 8 ) => bar.preset;
    std.rand2f( 0, 1 ) => bar.stickHardness;
    std.rand2f( 0, 1 ) => bar.strikePosition;
    std.rand2f( 0, 1 ) => bar.vibratoGain;
    std.rand2f( 0, 60 ) => bar.vibratoFreq;
    std.rand2f( 0, 1 ) => bar.volume;
    std.rand2f( .5, 1 ) => bar.directGain;
    std.rand2f( .5, 1 ) => bar.masterGain;

    <<< "---", "" >>>;
    <<< "preset:", bar.preset() >>>;
    <<< "stick hardness:", bar.stickHardness() >>>;
    <<< "strike position:", bar.strikePosition() >>>;
    <<< "vibrato gain:", bar.vibratoGain() >>>;
    <<< "vibrato freq:", bar.vibratoFreq() >>>;
    <<< "volume:", bar.volume() >>>;
    <<< "direct gain:", bar.directGain() >>>;
    <<< "master gain:", bar.masterGain() >>>;

    // set freq
	scale[std.rand2(0,scale.cap()-1)] => int winner;
    57 + std.rand2(0,2)*12 + winner => std.mtof => bar.freq;
    // go
    .8 => bar.noteOn;

    // advance time
    .5::second => now;
}

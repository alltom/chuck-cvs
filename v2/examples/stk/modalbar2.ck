// STK ModalBar

// patch
ModalBar bar => dac;

// scale
[0, 2, 4, 7, 8, 11] @=> int scale[];

// infinite time loop
while( true )
{
    // ding!
    std.rand2( 0, 8 ) => int preset;
    std.rand2f( 0, 128 ) => float stickHardness;
    std.rand2f( 0, 128 ) => float strikePosition;
    std.rand2f( 0, 128 ) => float vibratoGain;
    std.rand2f( 0, 128 ) => float vibratoFreq;
    std.rand2f( 0, 128 ) => float volume;
    std.rand2f( 64, 128 ) => float directGain;
    std.rand2f( 64, 128 ) => float masterGain;

    bar.controlChange( 2, stickHardness );
    bar.controlChange( 4, strikePosition );
    bar.controlChange( 11, vibratoGain );
    bar.controlChange( 7, vibratoFreq );
    bar.controlChange( 1, directGain);
    bar.controlChange( 128, volume );
    bar.controlChange( 16, preset );

    <<< "---", "" >>>;
    <<< "preset:", preset >>>;
    <<< "stick hardness:", stickHardness, "/ 128.0" >>>;
    <<< "strike position:", strikePosition, "/ 128.0" >>>;
    <<< "vibrato gain:", vibratoGain, "/ 128.0" >>>;
    <<< "vibrato freq:", vibratoFreq, "/ 128.0" >>>;
    <<< "volume:", volume, "/ 128.0" >>>;
    <<< "direct gain:", directGain, "/ 128.0" >>>;
    <<< "master gain:", masterGain, "/ 128.0" >>>;

    // set freq
	scale[std.rand2(0,scale.cap()-1)] => int winner;
    57 + std.rand2(0,2)*12 + winner => std.mtof => bar.freq;
    // go
    .8 => bar.noteOn;

    // advance time
    .5::second => now;
}

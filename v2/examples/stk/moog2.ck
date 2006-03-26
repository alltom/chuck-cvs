// STK ModalBar

// patch
Moog moog => dac;

// scale
[0, 2, 4, 7, 8, 11] @=> int scale[];

// infinite time loop
while( true )
{
    // ding!
    std.rand2f( 0, 128 ) => float filterQ;
    std.rand2f( 0, 128 ) => float filterSweep;
    std.rand2f( 0, 128 ) => float vol;
    std.rand2f( 0, 128 ) => float vibratoFreq;
    std.rand2f( 0, 128 ) => float vibratoGain;

    moog.controlChange( 2, filterQ);
    moog.controlChange( 4, filterSweep);
    moog.controlChange( 11, vibratoFreq);
    moog.controlChange( 1, vibratoGain);
    moog.controlChange( 128, vol);

    <<< "---", "" >>>;
    <<< "filterQ", moog.filterQ() >>>;
    <<< "filter sweep rate:", moog.filterSweepRate() >>>;
    <<< "mod speed:", moog.lfoSpeed() >>>;
    <<< "mod depth:", moog.lfoDepth() >>>;
    <<< "volume:", moog.volume() >>>;
    <<< "vibrato freq:", moog.vibratoFreq() >>>;
    <<< "vibrato gain:", moog.vibratoGain() >>>;

    // set freq
    scale[std.rand2(0,scale.cap()-1)] => int winner;
    57 + std.rand2(0,2)*12 + winner => std.mtof => moog.freq;
    // go
    .8 => moog.noteOn;

    // advance time
    .5::second => now;
}

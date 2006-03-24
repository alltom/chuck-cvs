// STK ModalBar

// patch
Moog moog => dac;

// scale
[0, 2, 4, 7, 8, 11] @=> int scale[];

// infinite time loop
while( true )
{
    // ding!
    std.rand2f( 0, 1 ) => float filterQ;
    //filterQ => moog.filterQ;
    std.rand2f( 0, 1 ) => float filterSweep;
    filterSweep => moog.filterSweepRate;
    std.rand2f( 0, 1 ) => float modSpeed;
    modSpeed => moog.modSpeed;
    std.rand2f( 0, 60 ) => float modDepth;
    modDepth => moog.modDepth;
    std.rand2f( 0, 1 ) => float vol;
    vol => moog.volume;
    std.rand2f( 0, 1 ) => float vibratoFreq;
    vibratoFreq => moog.vibratoFreq;
    std.rand2f( 0, 1 ) => float moog.vibratoGain;

    <<< "---", "" >>>;
    <<< "filterQ", moog.filterQ() >>>;
    <<< "filter sweep rate:", moog.filterSweepRate() >>>;
    <<< "mod speed:", moog.modSpeed() >>>;
    <<< "mod depth:", moog.modDepth() >>>;
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

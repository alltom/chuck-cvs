// STK BandedWG

// patch
BandedWG bwg => dac;

// scale
[0, 2, 4, 7, 8, 11] @=> int scale[];

// infinite time loop
while( true )
{
    // ding!
    std.rand2f( 0, 1 ) => bwg.pluck;
    std.rand2f( 0, 128 ) => float bowPressure;
    std.rand2f( 0, 128 ) => float bowMotion;
    std.rand2f( 0, 128 ) => float strikePosition;
    std.rand2f( 0, 128 ) => float vibratoFreq;
    std.rand2f( 0, 128 ) => float gain;
    std.rand2f( 0, 128 ) => float bowVelocity;
    std.rand2f( 0, 128 ) => float setStriking;
    std.rand2f( 0, 3 ) => float preset;

    std.rand2f( 0, 1) => bwg.bowRate;

    bwg.controlChange( 2, bowPressure);
    bwg.controlChange( 4, bowMotion);
    bwg.controlChange( 8, strikePosition);
    bwg.controlChange( 11, vibratoFreq);
    bwg.controlChange( 1, gain);
    bwg.controlChange( 128, bowVelocity);
    bwg.controlChange( 64, setStriking);
	
    <<< "---", "" >>>;
    <<< "strike position:", bwg.strikePosition() >>>;
    <<< "bow rate:", bwg.bowRate() >>>;
    <<< "bow Pressure:", bwg.bowPressure() >>>;

    // set freq
	scale[std.rand2(0,scale.cap()-1)] => int winner;
    57 + std.rand2(0,2)*12 + winner => std.mtof => bwg.freq;
    // go
    .8 => bwg.noteOn;

    // advance time
    1::second => now;
    1.0 => bwg.noteOff;
    .5::second => now;

    <<< "pluck -> bow", "" >>>;

    .8 => bwg.startBowing;
    1::second => now;
    1.0 => bwg.stopBowing;

    <<< "--", "" >>>;
    bwg.controlChange(16, std.rand2(0, 3));

    <<< "preset:", bwg.preset() >>>;    
    <<< "strike position:", bwg.strikePosition() >>>;
    <<< "bow rate:", bwg.bowRate() >>>;
    <<< "bow Pressure:", bwg.bowPressure() >>>;

    // set freq
    57 + std.rand2(0,2)*12 + winner => std.mtof => bwg.freq;
    // go
    .8 => bwg.noteOn;

    // advance time
    1::second => now;
    1.0 => bwg.noteOff;
    .5::second => now;

    <<< "pluck -> bow", "" >>>;

    .8 => bwg.startBowing;
    1::second => now;
    1.0 => bwg.stopBowing;

}

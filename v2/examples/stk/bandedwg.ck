// STK BandedWG

// patch
BandedWG bwg => dac;

// scale
[0, 2, 4, 7, 8, 11] @=> int scale[];

// infinite time loop
while( true )
{
    // ding!
    std.rand2f( 0, 1 ) => bwg.bowRate;
    std.rand2f( 0, 1 ) => bwg.bowPressure;
    std.rand2f( 0, 1 ) => bwg.strikePosition;
    std.rand2(0, 3) => bwg.preset;

    // print some parameters
    <<< "---", "" >>>;
    <<< "preset:", bwg.preset() >>>;    
    <<< "strike position:", bwg.strikePosition() >>>;
    <<< "bow rate:", bwg.bowRate() >>>;
    <<< "bow Pressure:", bwg.bowPressure() >>>;
    <<< "---", "" >>>;

    // set freq
    scale[std.rand2(0,scale.cap()-1)] => int winner;
    57 + std.rand2(0,2)*12 + winner => std.mtof => bwg.freq;

    <<< "bow -> pluck", "" >>>;
    std.rand2f( .5, 1 ) => bwg.pluck;
    // advance time
    2::second => now;

    <<< "pluck -> bow", "" >>>;
    .8 => bwg.startBowing;
    std.rand2f(2,4)::second => now;
    1.0 => bwg.stopBowing;
    1::second => now;
}

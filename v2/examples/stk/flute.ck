// STK Flute

// patch
Flute flute => PoleZero f => JCRev r => dac;
.75 => r.gain;
.05 => r.mix;
1 => f.blockZero;

// our notes
[ 61, 63, 65, 66, 68 ] @=> int notes[];

// infinite time-loop
while( true )
{
    // clear
    flute.clear( 1.0 );

    std.rand2f( 0, 1 ) => flute.jetDelay;
    std.rand2f( 0, 1 ) => flute.jetReflection;
    std.rand2f( 0, 1 ) => flute.endReflection;
    std.rand2f( 0, 1 ) => flute.noiseGain;
    std.rand2f( 0, 12 ) => flute.vibratoFreq;
    std.rand2f( 0, 1 ) => flute.vibratoGain;
    std.rand2f( 0, 1 ) => flute.pressure;

    <<< "---", "" >>>;
    <<< "jetDelay:", flute.jetDelay() >>>;
    <<< "jetReflection:", flute.jetReflection() >>>;
    <<< "endReflection:", flute.endReflection() >>>;
    <<< "noiseGain:", flute.noiseGain() >>>;
    <<< "vibratoFreq:", flute.vibratoFreq() >>>;
    <<< "vibratoGain:", flute.vibratoGain() >>>;
    <<< "breath pressure:", flute.pressure() >>>;

    // factor
    std.rand2f( .75, 2 ) => float factor;

    for( int i; i < notes.cap(); i++ )
    {
        play( 12 + notes[i], std.rand2f( .6, .9 ) );
        300::ms * factor => now;
    }
}

// basic play function (add more arguments as needed)
fun void play( float note, float velocity )
{
    // start the note
    std.mtof( note ) => flute.freq;
    velocity => flute.noteOn;
}

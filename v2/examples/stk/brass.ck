// patch
Brass brass => JCRev r => dac;
.75 => r.gain;
.1 => r.mix;

// our notes
[ 61, 63, 65, 66, 68 ] @=> int notes[];

// infinite time-loop
while( true )
{
    std.rand2f( 0, 1 ) => brass.lip;
    std.rand2f( 0, 1 ) => brass.slide;
    std.rand2f( 0, 12 ) => brass.vibratoFreq;
    std.rand2f( 0, 1 ) => brass.vibratoGain;
    std.rand2f( 0, 1 ) => brass.volume;

    <<< "---", "" >>>;
    <<< "lip tension:", brass.lip() >>>;
    <<< "slide length:", brass.slide() >>>;
    <<< "vibrato freq:", brass.vibratoFreq() >>>;
    <<< "vibrato gain:", brass.vibratoGain() >>>;
    <<< "volume:", brass.volume() >>>;

    for( int i; i < notes.cap(); i++ )
    {
        play( 12 + notes[i], std.rand2f( .6, .9 ) );
        300::ms => now;
    }
}

// basic play function (add more arguments as needed)
fun void play( float note, float velocity )
{
    // start the note
    std.mtof( note ) => brass.freq;
    velocity => brass.noteOn;
}

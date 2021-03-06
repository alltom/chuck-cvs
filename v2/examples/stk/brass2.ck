// patch
Brass brass => JCRev r => dac;
.75 => r.gain;
.1 => r.mix;

// our notes
[ 61, 63, 65, 66, 68 ] @=> int notes[];

// infinite time-loop
while( true )
{
    Std.rand2f( 64, 128 ) => float tension;
    Std.rand2f( 0, 128 ) => float length;
    Std.rand2f( 0, 128 ) => float vibratofreq;
    Std.rand2f( 0, 128 ) => float vibratogain;
    Std.rand2f( 64, 128 ) => float volume;

    <<< "---", "" >>>;
    <<< "lip tension:", tension >>>;
    <<< "slide length:", length >>>;
    <<< "vibrato freq:", vibratofreq >>>;
    <<< "vibrato gain:", vibratogain >>>;
    <<< "volume:", volume >>>;

    // lip tension
    brass.controlChange( 2, tension );
    // slide length
    brass.controlChange( 4, length );
    // vibrato freq
    brass.controlChange( 11, vibratofreq );
    // vibrato gain
    brass.controlChange( 1, vibratogain );
    // volume
    brass.controlChange( 128, volume );

    for( int i; i < notes.cap(); i++ )
    {
        play( 12 + notes[i], Std.rand2f( .6, .9 ) );
        300::ms => now;
    }
}

// basic play function (add more arguments as needed)
fun void play( float note, float velocity )
{
    // start the note
    Std.mtof( note ) => brass.freq;
    velocity => brass.noteOn;
}

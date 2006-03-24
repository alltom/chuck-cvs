// STK StifKarp

// patch
StifKarp m => JCRev r => dac;
.75 => r.gain;
.05 => r.mix;

// our notes
[ 61, 63, 65, 66, 68, 66, 65, 63 ] @=> int notes[];

// infinite time-loop
while( true )
{
    std.rand2f( 0, 1 ) => m.pickupPosition;
    std.rand2f( 0, 1 ) => m.sustain;
    std.rand2f( 0, 1 ) => m.stretch;

    <<< "---", "" >>>;
    <<< "pickup:", m.pickupPosition() >>>;
    <<< "sustain:", m.sustain() >>>;
    <<< "stretch:", m.stretch() >>>;

    // factor
    std.rand2f( 1, 4 ) => float factor;

    for( int i; i < notes.cap(); i++ )
    {
        play( std.rand2(0,2)*12 + notes[i], std.rand2f( .6, .9 ) );
        100::ms * factor => now;
    }
}

// basic play function (add more arguments as needed)
fun void play( float note, float velocity )
{
    // start the note
    std.mtof( note ) => m.freq;
    velocity => m.pluck;
}

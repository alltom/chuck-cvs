// STK Mandolin

// patch
Mandolin m => JCRev r => dac;
.75 => r.gain;
.05 => r.mix;

// our notes
[ 61, 63, 65, 66, 68, 66, 65, 63 ] @=> int notes[];

// infinite time-loop
while( true )
{
    std.rand2f( 0, 1 ) => m.bodySize;
    std.rand2f( 0, 1 ) => m.pluckPos;
    // std.rand2f( 0, 1 ) => m.stringDamping;
    // std.rand2f( 0, 1 ) => m.stringDetune;

    <<< "---", "" >>>;
    <<< "body size:", m.bodySize() >>>;
    <<< "pluck position:", m.pluckPos() >>>;
    <<< "string damping:", m.stringDamping() >>>;
    <<< "string detune:", m.stringDetune() >>>;

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

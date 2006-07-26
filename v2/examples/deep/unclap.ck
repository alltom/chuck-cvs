// name: unclap.ck
// desc: configurable "clapping music" (Steve Reich)
// author: Jesus Gollonet (original)
//         Ge Wang (shreds and glottal pops)
// date: Summer 2006

// our patch
sndbuf clapper1 => dac.left;
sndbuf clapper2 => dac.right;

// load built-in sounds
"special:glot_ahh" => clapper1.read;
"special:glot_ooo" => clapper2.read;

// the full "clapping music" figure
[.5, .5, 1, .5, 1, 1, .5, 1 ] @=> float seq[];

// length of quarter note
.36::second => dur quarter;
// how many measures per shift
3 => int shift_period;
// how much to shift by (in quarter notes)
.5 => float shift_factor;

// one clapper
fun void clap( sndbuf buffy, int max, float factor )
{
    1 => int shifts;

    // infinite time loop
    for( ; ; shifts++ )
    {
        // one measure
        for( 0 => int count; count < seq.cap(); count++ )
        {
            // set gain
            seq[count] * 2 => buffy.gain;
            // clap!
            0 => buffy.pos;
            // let time go by
            if( !max || shifts < max || count != (seq.cap() - 1) )
                seq[count]::quarter => now;
            else
            {
                <<< "shift!!!", "" >>>;
                seq[count]*factor*quarter => now;
                1 => shifts;
            }
        }
    }
}

// spork one clapper, shift every shift_period measures
spork ~ clap( clapper1, shift_period, shift_factor );
// spork, no shift
spork ~ clap( clapper2, 0, 0 );

// infinite time loop
while( true ) 1::day => now;

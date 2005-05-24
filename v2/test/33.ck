
sinosc s => dac;

[ 0, 2, 4, 7, 9 ] @=> int f[];

while( true )
{
//    2000 => int i;
//    while( i > 0 )
//    {
        std.mtof( f[math.rand2( 0, 4 )] + 60 ) => s.freq;
        // Math.rand2f( .2, .5 ) => s.gain;
//        1 -=> i;
//    }
    100::ms => now;
}


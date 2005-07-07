// sine tones (fail-correct)
sinosc s => dac;

[ 0, 2, 4, 7, 9 ] @=> int f[];

while( true )
{
    std.mtof( f[math.rand2( 0, 4 )] + 72 ) => s.freq;
    100::ms => now;
}

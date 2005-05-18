
sinosc s => dac;

[ 0, 2, 4, 7, 9 ] @=> int f[];

while( true )
{
    10000 => int i;
    while( i > 0 )
    {
        Std.mtof( f[Math.rand2( 0, 4 )] + 34) => s.freq;
        1 -=> i;
    }
    100::ms => now;
}

Mandolin mand => dac;
220.0 => mand.freq;

while( true )
{
    std.rand2f( 0.4, 0.9 ) => mand.pluck;
    if ( std.randf() > 0.3 )
    { 1::second => now; }
    else
    { 0.5::second => now; }

    std.rand2f ( 0.2, 0.8 ) => mand.pluckPos;
    220.0 * math.pow( 1.059, (float)std.rand2(0,24) ) => mand.freq;
}

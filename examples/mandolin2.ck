Mandolin mand => dac;
220.0 => mand.freq;

while( true )
{
    std.rand2f( 0.2, 0.9 ) => mand.pluck;

    if( std.randf() > 0.7 )
    { 500::ms => now; }
    else if( std.randf() > .7 )
    { 250::ms => now; }
    else if( std.randf() > -0.8 )
    { .125::second => now; }
    else
    {
        0 => int i;
        4 * std.rand2( 1, 4 ) => int pick;
        0 => int pick_dir;
        0.0 => float pluck;
        for( ; i < pick; i++ )
        {
            std.rand2f(.2,.3) + (float)i*.035 => pluck;
            pluck + -.15 * (float)pick_dir => mand.pluck;
            !pick_dir => pick_dir;
            75::ms => now;
        }
    }

    std.rand2f( 0.2, 0.8 ) => mand.pluckPos;
    // pentatonic
    2 * std.rand2( 0, 4 ) => int freq;
    if( freq == 6 ) 7 => freq;
    if( freq == 8 ) 9 => freq;
    220.0 * math.pow( 1.059, (float)(std.rand2(0,1)*12)+(float)freq ) => mand.freq; 
}

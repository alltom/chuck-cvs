// banded waveguide sample

BandedWG band => JCRev r => dac;

0.95 => band.gain;
1 => band.preset;
std.mtof( (float)( 21 + std.rand2(0,3) * 12 + 7 ) ) => band.freq;
.8 => r.gain;
.1 => r.mix;
// our main loop
while( true )
{    
    std.rand2f( 0.1, 0.3 ) => band.bowRate;
    std.rand2f( 0.4, 0.5 ) => band.bowPressure;
    std.rand2f( 0.6, 0.8 ) => band.startBowing;

    if( std.randf() > 0.7 )
    { 1000::ms => now; }
    else if( std.randf() > .7 )
    { 500::ms => now; }
    else if( std.randf() > -0.5 )
    { .250::second => now; }
    else
    {
        0 => int i;
        4 * std.rand2( 1, 4 ) => int pick;
        0 => int pick_dir;
        0.0 => float pluck;
        std.rand2f( 50.0, 200.0 ) => float d;

	for( ; i < pick; i++ )
        {
            std.rand2f(.4,.6) + (float)i*.35/(float)pick => pluck;
            pluck + 0.1 * (float)pick_dir => band.pluck;
            !pick_dir => pick_dir;
            d::ms => now;
        }
    }

    if ( std.randf() > 0.6 ) { 
        1::second => now;
        0.001 => band.stopBowing;
        0.5::second * (float) std.rand2(1,3) => now;

        // pentatonic
        2 * std.rand2( 0, 4 ) => int freq;
        if( freq == 6 ) 7 => freq;
        if( freq == 8 ) 9 => freq;
        std.mtof( (float)( 21 + std.rand2(0,5) * 12 + freq ) ) => band.freq;
        if ( std.randf() > 0.7 ) 
            std.rand2(0,3) => band.preset;
    }
}

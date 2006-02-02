KBHit kb;

impulse i => biquad f => dac;
// set the filter's pole radius
.99 => f.prad;
// set equal gain zeros
1 => f.eqzs;
// initialize float variable
0.0 => float v;
// set filter gain
.5 => f.gain;

int c;
while( true )
{
    kb => now;

    1.0 => i.next;
    while( kb.more() )
    {
        kb.getchar() => c;
        std.mtof( c ) => f.pfreq;
        <<< c >>>;
    }
}


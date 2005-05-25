
MidiOut mout;
MidiMsg mg;
MidiRW mrw;

if( !mout.open( 1 ) )
    <<<"bad">>>;

sndbuf s => dac;

mrw.open( "z.txt" );
"foo.wav" => s.read;
time t;
now => t;

while( mrw.read( mg ) > 0 )
{
    //mg.when - t => now;
    //mg.when => t;
    mg.when => now;
    mout.send( mg );
    //<<< mg.data1 >>>;
    //<<< mg.data2 >>>;
    //<<< mg.data3 >>>;
    //<<< mg.when >>>;
    //<<< "----" >>>;
}

mrw.close();

//<<<mrw.read( mg, t )>>>;

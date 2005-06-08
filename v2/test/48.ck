
MidiOut mout;
MidiMsg mg;
MidiMsgIn mrw;

if( !mout.open( 0 ) )
    <<<"bad">>>;

//sinosc s => dac;
Mandolin m => JCRev r => dac;
.2 => r.mix;

mrw.open( "z.txt" );
//"foo.wav" => s.read;

0.5 => float scale;

time t;
now => t;

int count;

while( mrw.read( mg ) != 0 )
{
    <<<"ha">>>;
    scale * (mg.when - t) => now;
    mg.when => t;
    //mg.when => now;
    mout.send( mg );
    std.rand2f( .8, .9 ) => m.pluckPos;
    std.mtof( mg.data2 ) => m.freq;
    mg.data3 / 128.0 => m.pluck;
    //<<< mg.data1 >>>;
    //<<< mg.data2 >>>;
    //<<< mg.data3 >>>;
    <<< mg.when >>>;
    //<<< "----" >>>;

    <<<count + 1 => count>>>;
}

mrw.close();

//<<<mrw.read( mg, t )>>>;

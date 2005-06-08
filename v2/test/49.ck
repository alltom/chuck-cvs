//------------------------------------------------
// name: polyfony.ck
// desc: polyfonic midi
//
// by: Ananya Misra and Ge Wang
// send all complaints to prc@cs.princeton.edu
//--------------------------------------------

MidiMsg msg;

class NoteEvent extends event
{
    int note;
    int velocity;
}

// the event
NoteEvent on;
// array of ugen's handling each note
event us[128];

gain g => dac;
.1 => g.gain;

fun void handler()
{
    // don't connect to dac until we need it
    Mandolin m;
    PRCRev r => dac;
    .2 => r.mix;
    event off;
    int note;

    while( true )
    {
        on => now;
        on.note => note;
        // dynamically repatch
        m => r;
        std.mtof( note ) => m.freq;
        std.rand2f( .6, .8 ) => m.pluckPos;
        on.velocity / 128.0 => m.pluck;
        off @=> us[note];

        off => now;
        null @=> us[note];
        m =< r;
    }
}

// spork handlers
for( 0 => int i; i < 10; i++ ) spork ~ handler();

MidiMsgIn mrw;

mrw.open( "zz.txt" );

1.0 => float scale;

time t;
now => t;

// get the midimsg
while( mrw.read( msg ) != 0 )
{
    <<<scale, msg.when, t>>>;
    <<<scale * (msg.when - t)>>> => now;
    <<<"hi">>>;
    msg.when => t;

    if( msg.data1 != 144 )  // !noteon
        continue;

    <<<1>>>;
    if( msg.data3 > 0 )
    {
    <<<2>>>;
        // store midi note number
        msg.data2 => on.note;
        // store velocity
        msg.data3 => on.velocity;
        // signal the event
        on.signal();
        // yield without advancing time to allow shred to run
        // me.yield();
    <<<3>>>;
    }
    else
    {
    <<<4>>>;
        if( us[msg.data2] != null ) us[msg.data2].signal();
    <<<5>>>;
    }
    <<<6>>>;
}
    <<<7>>>;

mrw.close();

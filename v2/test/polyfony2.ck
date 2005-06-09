//------------------------------------------------
// name: polyfony.ck
// desc: polyfonic midi
//
// by: Ananya Misra and Ge Wang
// send all complaints to prc@cs.princeton.edu
//--------------------------------------------

MidiIn min;
MidiMsg msg;

if( !min.open( 1 ) ) me.exit();

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
    Clarinet c;
    PRCRev r => g;
    .2 => r.mix;
    event off;
    int note;

    while( true )
    {
        on => now;
        on.note => note;
        // dynamically repatch
        c => r;
        std.mtof( note ) => c.freq;
        .4 + on.velocity / 128.0 * .6 => c.startBlowing;
        off @=> us[note];

        off => now;
        c.stopBlowing( 10.0 );
        null @=> us[note];
        100::ms => now;
        c =< r;
    }
}

// spork handlers
for( 0 => int i; i < 10; i++ ) spork ~ handler();

while( true )
{
    // wait on midi event
    min => now;

    // get the midimsg
    while( min.recv( msg ) )
    {
        if( msg.data1 != 144 )  // !noteon
            continue;

        if( msg.data3 > 0 )
        {
            // store midi note number
            msg.data2 => on.note;
            // store velocity
            msg.data3 => on.velocity;
            // signal the event
            on.signal();
            // yield without advancing time to allow shred to run
            me.yield();
        }
        else
        {
            if( us[msg.data2] != null ) us[msg.data2].signal();
        }
    }
}
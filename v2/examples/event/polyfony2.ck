//------------------------------------------------
// name: polyfony.ck
// desc: polyfonic midi
//
// by: Ananya Misra and Ge Wang
// send all complaints to prc@cs.princeton.edu
//--------------------------------------------

MidiIn min;
MidiMsg msg;

// try to open MIDI port (see chuck --probe for available devices)
if( !min.open( 0 ) ) me.exit();

// make our own event
class NoteEvent extends Event
{
    int note;
    int velocity;
}

// the event
NoteEvent on;
// array of ugen's handling each note
Event us[128];

// the base patch
gain g => JCRev r => dac;
.1 => g.gain;
.2 => r.mix;

// handler shred for a single voice
fun void handler()
{
    // don't connect to dac until we need it
    Clarinet c;
    Event off;
    int note;

    // inifinite time loop
    while( true )
    {
        on => now;
        on.note => note;
        // dynamically repatch
        c => g;
        std.mtof( note ) => c.freq;
        .4 + on.velocity / 128.0 * .6 => c.startBlowing;
        off @=> us[note];

        off => now;
        c.stopBlowing( 10.0 );
        null @=> us[note];
        100::ms => now;
        c =< g;
    }
}

// spork handlers, one for each voice
for( 0 => int i; i < 20; i++ ) spork ~ handler();

// infinite time loop
while( true )
{
    // wait on midi event
    min => now;

    // get the midimsg
    while( min.recv( msg ) )
    {
        // catch only noteon
        if( msg.data1 != 144 )
            continue;

        // check velocity
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

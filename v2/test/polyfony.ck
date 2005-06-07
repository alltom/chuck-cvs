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

int count;

fun void handler()
{
    count++;
    Mandolin m;
    JCRev r;
    if( count < 11 ) m => r => g;
    .2 => r.mix;
    0 => m.gain;
    event off;
    int note;

    while( true )
    {
        on => now;
        on.note => note;
        std.mtof( note ) => m.freq;
        std.rand2f( .6, .8 ) => m.pluckPos;
        1 => m.gain;
        on.velocity / 128.0 => m.pluck;
        off @=> us[note];

        off => now;
        null @=> us[note];
        0 => m.gain;
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
        if( msg.data3 > 0 )
        {
            // store midi note number
            msg.data2 => on.note;
            // store velocity
            msg.data3 => on.velocity;
            // signal the event
            on.signal(); 1::samp => now;
        }
        else
        {
            if( us[msg.data2] != null ) us[msg.data2].signal();
        }
    }
}

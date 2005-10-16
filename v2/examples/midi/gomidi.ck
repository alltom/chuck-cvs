// print out MIDI messages on device
0 => int device;

// the event
MidiIn min;
// the message
MidiMsg msg;

// try to open
if( !min.open( device ) ) me.exit();

// infinite event loop
while( true )
{
    // wait on event
    min => now;

    // get the messages
    while( min.recv(msg) )
    {
        // print out midi message
        <<< msg.data1, msg.data2, msg.data3 >>>;
    }
}

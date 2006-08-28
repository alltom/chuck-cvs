// launch with r.ck

// send object
OscSend xmit;

// aim the transmitter at our local port 6449
xmit.setHost( "localhost", 6449 );

// infinite time loop
while( true )
{
    int i; float f;

    // start the message...
    // the type string 'i f' expects a int, float
    xmit.startMsg( "/foo/notes", "i f" );

    // a message is kicked as soon as it is complete 
    // - type string is satisfied and bundles are closed
    Std.rand2( 30, 80 ) => xmit.addInt;
    Std.rand2f( .1, .5 ) => xmit.addFloat;

    // advance time
    0.2::second => now;
}

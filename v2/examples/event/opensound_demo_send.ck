// launch with opensound_demo_recv.ck

// send object
OSC_Send xmit;

// aim the transmitter at our local port 6449
xmit.setHost ( "localhost", 6449 );

// infinite time loop
while( true )
{
    // start the message...
    // the type string ',f' expects a single float argument
    xmit.startMesg( "/sndbuf/buf/rate", "f" );

    // a message is kicked as soon as it is complete 
    // - type string is satisfied and bundles are closed
    std.rand2f( .5, 2.0 ) => float temp => xmit.addFloat;
    <<< "sent (via OSC):", temp >>>;

    // advance time
    0.2::second => now;
}

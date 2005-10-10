// launch with opensound_demo_recv.ck

// send object
OSC_Send xmit;

// aim the transmitter at our local port 6449
xmit.setHost ( "localhost", 6449 );

// our notes
[0, 2, -44, 4, 2, 7, 9, 0, -3, -36] @=> int notes[];
10 => int nnote;
0 => int idx;

// infinite time loop
while( true )
{
    // start the message...
    // the type string ',f' expects a single float argument
    xmit.startMesg( "/sndbuf/buf/rate", ",f" );

    // a message is kicked as soon as it is complete 
    // - type string is satisfied and bundles are closed

    std.mtof( notes[idx] ) => xmit.addFloat;

    // increment our index
    (idx + 1) % nnote => idx;

    // let note play
    0.01::second => now;
}

// (launch with opensound_demo_send.ck)

// the patch
sndbuf buf => dac;
// load the file
"../data/snare.wav" => buf.read;
// don't play yet
0 => buf.play; 

// create our OSC receiver
OSC_Recv orec;
// use port 6449
6449 => orec.port;
// start listening (launch thread)
orec.listen();


// create an address in the receiver, store in new variable
orec.event("/sndbuf/buf/rate","f") @=> OSC_Addr rate_addr; 

// infinite event loop
while ( true )
{
    // wait for event to arrive
    rate_addr => now; //wait for Events to arrive.

    // grab the next message from the queue. 
    while ( rate_addr.nextMesg() != 0 )
    { 
        // getFloat fetches the expected float (as indicated by ",f")
        rate_addr.getFloat() => buf.play;
        // print
        <<< "got (via OSC):", buf.play() >>>;
        // set play pointer to beginning
        0 => buf.pos;
    }
}

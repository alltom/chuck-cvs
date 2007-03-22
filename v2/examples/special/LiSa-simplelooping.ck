//-----------------------------------------------------------------------------
// name: LiSa
// desc: Live sampling utilities for ChucK
//
// author: Dan Trueman, 2007
//
// to run (in command line chuck):
//     %> chuck LiSa_readme.ck
//
// to run (in miniAudicle):
//     ??
//-----------------------------------------------------------------------------

/*

Another simple example of LiSa, demonstrating dopey looping....

*/

//-----------------------------------------------------------------------------
//signal chain; record a sine wave, play it back
SinOsc s => LiSa loopme => dac;
//s => dac;
440. => s.freq;
0.2 => s.gain;

//alloc memory
1::second => loopme.duration;
loopme.recRamp(200::ms);

//start recording input
loopme.record(1);

//stop recording 1 second later, start playing what was just recorded
1000::ms => now;

loopme.record(0);

//set playback rate
loopme.rate(1.5);
loopme.play(1);
loopme.loop(1);
loopme.bi(1);

while(true) {500::ms => now;}

//bye bye

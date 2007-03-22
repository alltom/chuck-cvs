//-----------------------------------------------------------------------------
// name: LiSaBasic and LiSaMulti
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

based on the smelt envelope follower trigger program will trigger start and stop 
to one-shot LiSa buffers. a sort of triggered delay line, so the delays follow 
the player rather than being static. Success depends on tuning the envelope 
follower closely to the input gains on however you are getting audio into ChucK.
Mileage will vary....

*/
//-----------------------------------------------------------------------------

// patch
adc => Gain g => OnePole p => blackhole;
// square the input, by chucking adc to g a second time
adc => g;
// set g to multiply its inputs
3 => g.op;

// set pole position, influences how closely the envelope follows the input
//   : pole = 0 -> output == input; 
//   : as pole position approaches 1, follower will respond more slowly to input
0.995 => p.pole;
// thresholds
.0001 => float threshold;
.00005 => float releaseThresh;

// duration between successive polling
10::ms => dur pollDur;

//LiSa stuff
adc => LiSa l => dac;
l.duration(25::second);
//l.gain(0.2);
l.recRamp(20::ms);
l.record(1);

-1 => int voice;
float rate;

//read command line args; set rate
//be nice to have pitch follower determine this, or some kind of table....
for (0=>int i; i < me.args(); i++)
{
	Std.atof(me.arg(i)) => rate;
}
if (rate == 0.) 1. => rate;
<<<"setting rate to: ", rate>>>;

Event attacks[10];
dur newstarttime, newlen;
time starttime_real;

// infinite time loop
while( true )
{
    // detect onset
    if( p.last() > threshold )
    {
        // do something
        <<<"attack!; starting voice", voice>>>;
        //play last sample
        if(voice > -1) attacks[voice].signal();
        l.recPos() => newstarttime;
        now => starttime_real;
        
		//wait for release
        while(p.last() > releaseThresh) { pollDur => now; }
        <<<"release...">>>;

		//spork off new sample
		now - starttime_real => newlen;
        l.getVoice() => voice;
        if(voice > -1) spork ~ playlast(attacks[voice], newstarttime, newlen, rate, voice);

    }
    
    // determines poll rate
    pollDur => now;
}

fun void playlast(Event on, dur starttime, dur len, float newrate, int myvoice) 
{

	if (newrate == 0.) 1. => newrate;
    <<<"sporking shred with rate: ", newrate>>>;
    if(rate > 0.) l.playPos(myvoice, starttime);
    else l.playPos(myvoice, l.recPos() - 1::ms);
    
    on => now;

    l.rate(myvoice, newrate);
    l.rampUp(myvoice, 20::ms);
    
    Std.fabs(newrate) => float absrate;
    len / absrate => now;
    l.rampDown(myvoice, (250/absrate)::ms);
    (250/absrate)::ms => now;
    
    //bye bye shred....
}


	

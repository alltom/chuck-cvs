// the patch 

sndbuf buf => dac;
// load the file

"data/snare.wav" => buf.read;

// set the playback rate (1.0 is default)
OSC_Recv orec;
6449 => orec.port;
orec.listen();
function void ratecontroller() { 
	orec.event("/sndbuf/buf/play,f") @=> OSC_Addr ratemesg;
	OSC_Addr xfoo;
	while ( true ) { 
		ratemesg => now;
		while ( ratemesg.nextMesg() != 0 ) { 
			buf.play( ratemesg.getFloat() );
		}
	}	
}

1 => buf.loop;
1.0 => buf.play; 
spork ~ ratecontroller();

// time loop
while( true )
{
	1::second => now;
}

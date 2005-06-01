// the patch 

sndbuf buf => dac;
// load the file

"data/snare.wav" => buf.read;

// set the playback rate (1.0 is default)
OSC_Recv orec;
6449 => orec.port;
orec.listen();
function void ratecontroller() { 
	OSC_Addr ratemesg;
	"/sndbuf/buf/play,f" => ratemesg.set;
	ratemesg => orec.add_address;
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

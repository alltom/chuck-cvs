// dut dut dut dut dut dut doot doot doot doot doot doot doot

Wurley left=> gain g => dac ;
Rhodey right=> gain g => dac ;
220.0 => left.freq;
0.95 => left.gain;
220.0 => right.freq;
0.95 => right.gain;

0.3 => g.gain;

0.2 => float mag;
333::ms => dur onebeat;


//yay!

0.0 => float transp;

fun void fingers( float ln, float rn ) { 
	std.mtof(transp + ln) => left.freq;
	mag => left.noteOn;
	std.mtof(transp + rn) => right.freq;
	mag => right.noteOn;
}

0 => int i;
// our main loop
while( true )
{

	//6x f, g    65 67
	for ( 0=>i ; i < 6 ; i++ ) { 
		fingers(65.0, 67.0);
		onebeat => now;
	}

	//6x e, g    64 67
	for ( 0=>i ; i < 6 ; i++ ) { 
		fingers(64.0, 67.0 );
		onebeat => now;
	}

	//6x d, b    62 71
	for ( 0=>i ; i < 6 ; i++ ) { 
		fingers(62.0, 71.0 );
		onebeat => now;
	}

	//4x  c, c    60 72
	for ( 0=>i  ; i < 4 ; i++ ) { 
		fingers(60.0, 72.0 );
		onebeat => now;
	}

	if ( std.rand2(0, 4) > 2 ) { 		
	        2 * std.rand2( -3, 3 ) => int freq;
	        if( freq == 6 ) 5 => freq;
		if( freq  < 0 ) freq + 1 => freq ;
		(float)freq => transp;
		onebeat * 2.0 => now;
	}
	else { 
		// d,b 
		fingers(62.0, 71.0 );
		onebeat => now;

		// e,a
		fingers(64.0, 69.0 );
		onebeat => now;
	}

}

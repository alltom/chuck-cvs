// the patch 

OSC_Send xmit;
xmit.setHost ( "localhost", 6449 );

// time loop
0.0 => float running;
0.0 => float modrunning;
0 => int ct;
0.25 => float res;

while( true )
{
	float f;
	if ( ct % 4 != 3 ) { 
		std.rand2f( 0.90, 1.10 ) => f;
	}
	else { 
		running - modrunning => float diff;
		( 1.0 + diff ) / res => f; 
	}

	xmit.startMesg ( "/sndbuf/buf/play", ",f");
	f => xmit.addFloat;
	res * f::second => now;
	res * f +=> modrunning; //actual time
	res +=> running; //track time
	1 +=> ct;
}

Mandolin mand => dac;
440.0 => mand.freq;

while ( true ) { 

	std.rand2f(0.5, 0.9) => mand.pluck;
	if ( std.randf() >  0.3 ) { 
		1::second => now;
	}
	else { 
		0.5::second => now;
	}
	std.rand2f ( 0.2, 0.8 ) => mand.pluckPos;
	std.rand2f ( 440.0, 2000.0 ) => mand.freq;
	
}
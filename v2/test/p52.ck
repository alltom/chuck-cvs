// float comp
// test this using "time ../chuck p51.ck"
// ignore the fraction spent one the emitter

1.0 => float x;

if ( x == 1.0 ) { 
	1::second => now;
}

if ( x != 1.0 ) { 
	2::second => now;
}

if ( x < 1.0 ) { 
	4::second => now;
}

if ( x > 1.0 ) { 
	8::second => now;
}

if ( x >= 1.0 ) { 
	16::second => now;
}

if ( x <= 1.0 ) { 
	32::second => now;
}
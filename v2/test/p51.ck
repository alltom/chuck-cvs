// int comp
// test this using "time ../chuck p51.ck"
// ignore the fraction spent one the emitter

0 => int x;

if ( x == 1 ) { 
	1::second => now;
}

if ( x != 1 ) { 
	2::second => now;
}

if ( x <= 1 ) { 
	4::second => now;
}

if ( x >= 1 ) { 
	8::second => now;
}

if ( x > 1 ) { 
	16::second => now;
}

if ( x < 1 ) { 
	32::second => now;
}
// function overloading

function int howMany() { 
	return 0;
}

function int howMany( int a ) { 
	return 1;
}

function int howMany( int a, int b ) { 
	return 2;
}

if ( howMany() != 0 ) { 
	1::second => now;
}

if ( howMany( 1 ) ) { 
	2::second => now;
}

if ( howMany( 2 ) ) { 
	4::second => now;
}
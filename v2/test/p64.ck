// spork spork spork

fun void waits( int a ) { 
	a::second => now;
}

spork ~ waits(5);
spork ~ waits(10);
2::second => now;
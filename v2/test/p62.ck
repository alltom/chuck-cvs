// spork spork spork

fun void wait10() { 
	10::second => now;
}

spork ~ wait10();
2::second => now;
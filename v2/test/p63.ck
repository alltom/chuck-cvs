// spork spork spork

fun void wait5() { 
	5::second => now;
}

fun void wait10() { 
	10::second => now;
}

spork ~ wait5();
spork ~ wait10();
2::second => now;
//gen9 inharmonic sinusoidal lookup table
//ported to ChucK from rtcmix by dan trueman
//see luke's page: http://www.music.columbia.edu/cmix/makegens.html

Phasor drive => Gen9 g9 => dac;
//can also lookup table values directly:
//		g9.lookup(index);
//		with index:[0,1]

//load up the coeffs
//these are in triplets:
//		harmonic partial ratio, amplitude, phase
//		see the RTcmix page: 
//		http://www.music.columbia.edu/cmc/Rtcmix/docs/scorefile/gen9.html
//		for nice documentation of this gen

[1., 1., 0.,   2, 0.5, 90.,   3., 0.2, 180.] @=> float coefs[];
//[1., 1., 0.] @=> float coeffs[]; //a simple one
g9.coefs(coefs);
0.1 => g9.gain; //make it quiet

//set frequency for reading through table
drive.freq(440.);

//create an envelope to scan through the table values
//so we can look at them as well as hear them
Envelope e => blackhole;
e.duration(10000::ms);
0. => e.value;
e.keyOn();

while (true){

	<<<e.value(), g9.lookup(e.value())>>>;
		
	if(e.value() == 1.) break;

	10::ms => now;

}

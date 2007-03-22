//gen7: line segment table generator
//probably superceded by CurveTable
//ported to ChucK from rtcmix by dan trueman
//see luke's page: http://www.music.columbia.edu/cmix/makegens.html#03

Gen7 g7;
//set up the values [0,1] and distances in the table
//first arg is initial value 
//followed by pairs indicating distance (total distance = 1)
//and destination value for linear segments

[0., 0.5, 1., 0.5, 0.] @=> float values[];
g7.coefs(values);

//create an envelope to scan through the table values
Envelope e => blackhole;
e.duration(10000::ms);
0. => e.value;
e.keyOn();

while (true){	

	<<<e.value(), g7.lookup(e.value())>>>;
		
	if(e.value() == 1.) break;

	10::ms => now; 

}

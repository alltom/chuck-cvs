// chuck this with other shreds to record to file

dac => gain g => WvOut w => blackhole;
"foo.wav" => w.wavFilename;
.5 => g.gain;

while( true ) 1::second => now;

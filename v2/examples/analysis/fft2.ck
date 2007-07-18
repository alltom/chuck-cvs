// our patch
SinOsc g => FFT fft => blackhole;
// hear it
g => dac;
// set gain
.15 => g.gain;
// set srate
second / samp => float srate;

// set parameters
1024 => fft.size;

// use this to hold contents
complex s[fft.size()/2];
// divide
int div;

// control loop
while( true )
{
    // set freq
    srate / fft.size() * div++ => g.freq;
    fft.size()/2 %=> div;
    
    // take fft
    fft.upchuck();
    // get contents
    fft.spectrum( s );

    // advance time
    512::samp => now;
}

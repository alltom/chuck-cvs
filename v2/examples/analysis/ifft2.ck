// our patch
SinOsc g => FFT fft => blackhole;
// synthesis
IFFT ifft => dac;
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
    // take ifft
    ifft.transform( s );

    // advance time
    fft.size()::samp => now;
}

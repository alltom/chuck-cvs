// our patch (try replacing the SinOsc with adc)
SinOsc s => FFT fft =^ Centroid cent => blackhole;

// set parameters
512 => fft.size;
// set hann window
Windowing.hann(512) => fft.window;
// set sine frequency
440 => s.freq;
// compute srate
second / samp => float srate;

// control loop
while( true )
{
    // upchuck: take fft then centroid
    cent.upchuck() @=> UAnaBlob blob;
    // print out centroid
    <<< blob.fvals()[0] * srate / 2 >>>;

    // advance time
    fft.size()::samp => now;
}

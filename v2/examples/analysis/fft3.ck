// our patch
SinOsc g => FFT fft => blackhole;

// set srate
second / samp => float srate;
// set parameters
8 => fft.size;

// divide
int div;
// the blob
UAnaBlob blob;

// control loop
while( true )
{
    // set freq
    srate / fft.size() * div++ => g.freq;
    fft.size()/2 %=> div;
    
    // take fft
    fft.upchuck() @=> blob;
    // examine contents
    <<< blob.cvals()[0]$polar, blob.cvals()[1]$polar,
        blob.cvals()[2]$polar, blob.cvals()[3]$polar >>>;

    // advance time
    100::ms => now;
}

// really really bad cross synthesizer...

// our patch
adc => FFT fftx => blackhole;
// what to cross
FFT ffty => blackhole;
BlitSquare blt[6];
[ 66, 70, 73, 78, 84, 87] @=> int pitches[];
for( int i; i < blt.cap(); i++ )
{
    blt[i] => ffty;
    20 => blt[i].harmonics;
    pitches[i] => Std.mtof => blt[i].freq;
}
// synthesis
IFFT ifft => NRev rev => dac;
.02 => rev.mix;

// set FFT size
1024 => fftx.size => ffty.size => int FFT_SIZE;
// desired hop size
FFT_SIZE / 4 => int HOP_SIZE;
// set window and window size
Windowing.blackmanHarris(512) => fftx.window;
Windowing.blackmanHarris(512) => ffty.window;
Windowing.blackmanHarris(512) => ifft.window;

// use this to hold contents
complex X[FFT_SIZE/2];
complex Y[FFT_SIZE/2];
complex Z[FFT_SIZE/2];

// control loop
while( true )
{
    // take fft
    fftx.upchuck();
    ffty.upchuck();

    // get contents
    fftx.spectrum( X );
    ffty.spectrum( Y );

    // multiply
    for( int i; i < X.cap(); i++ )
        120 * X[i] * Y[i] => Z[i];

    // take ifft
    ifft.transform( Z );

    // advance time
    HOP_SIZE::samp => now;
}

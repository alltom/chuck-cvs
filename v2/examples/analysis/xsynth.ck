// really really bad cross synthesizer...

// our patch
adc => FFT X => blackhole;
FFT Y => blackhole;
// synthesis
IFFT ifft => dac;

// what to cross
BlitSquare blt[6];
[ 66, 70, 73, 78, 84, 87] @=> int pitches[];
for( int i; i < blt.size(); i++ )
{
    blt[i] => Y;
    20 => blt[i].harmonics;
    pitches[i] => Std.mtof => blt[i].freq;
}

// set FFT size
4096 => X.size => Y.size => int FFT_SIZE;
// desired hop size
FFT_SIZE / 8 => int HOP_SIZE;
// set window and window size
Windowing.hann(1024) => X.window;
Windowing.hann(1024) => Y.window;
// use this to hold contents
complex Z[FFT_SIZE/2];

// control loop
while( true )
{
    // take fft
    X.upchuck();
    Y.upchuck();
    
    // multiply
    for( int i; i < X.size()/2; i++ )
        Math.sqrt((X.cval(i)$polar).mag) * Y.cval(i) => Z[i];
    
    // take ifft
    ifft.transform( Z );
    
    // advance time
    HOP_SIZE::samp => now;
}

// the patch 
SndBuf buf => dac;
// load the file
"../data/snare.wav" => buf.read;

// time loop
while( true )
{
    0 => buf.pos;
    Std.rand2f(.2,.9) => buf.gain;
    Std.rand2f(.5,1.5) => buf.rate;
    100::ms => now;
}

// the patch 
sndbuf buf => dac;
// load the file (win32: may need to change / to \)
"data/snare.wav" => buf.read;

// play starting the next sample in ChucK time
1.0 => buf.play; 

// time loop
while( true )
{
    0 => buf.pos;
    std.rand2f(.2,.9) => buf.gain;
    100::ms => now;
}

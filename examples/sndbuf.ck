// the patch 
sndbuf buf => dac;
"data/snare.wav" => buf.read;
// play starting now
1.0 => buf.play; 

// time loop
while( true )
{
    0 => buf.pos;
    std.rand2f(.2,.9) => buf.gain;
    100::ms => now;
}

// the patch
sndbuf buf => dac;
// load
"snare.wav" => buf.read;
// play with rate 1.0
1.0 => buf.play;

// time loop
while( true )
{
    0 => buf.pos;
    std.rand2f(.2, .9) => buf.gain;
    100::ms => now;
}

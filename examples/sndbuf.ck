sndbuf buf => dac;
"snare.wav" => buf.read;
1.0 => buf.rate;

while( true )
{
    0 => buf.pos;
    100::ms => now;
}

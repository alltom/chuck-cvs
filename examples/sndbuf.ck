sndbuf buf => dac;
"snare.wav" => buf.read;

while( true )
{
    0 => buf.pos;
    100::ms => now;
}

sndbuf buf => dac;
"snare.raw" => buf.read;

while( true )
{
    0 => buf.pos;
    100::ms => now;
}

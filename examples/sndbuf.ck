sndbuf buf => dac;
"snare.raw" => buf.path;

while( true )
{
    0 => buf.pos;
    100::ms => now;
}

sinosc s => netout n => blackhole;
220.0 => s.freq;
"127.0.0.1" => n.addr => stdout;

while( true )
{
    100::ms => now;
}

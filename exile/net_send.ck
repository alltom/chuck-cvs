sinosc s => netout n => blackhole;
220.0 => s.freq;
"beanfuzz.dyndns.org" => n.addr;
8890 => n.port;

while( true )
{
    100::ms => now;
}

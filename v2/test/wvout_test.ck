
impulse i => WvOut w => blackhole;
	i => dac;

"special:auto" => w.aifFilename;
<<<w.filename()>>>;

// time loop
while( true )
{
	1.0 => i.next;
	2::second => now;
}

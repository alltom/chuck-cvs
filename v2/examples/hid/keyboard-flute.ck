HidIn hi;
HidMsg msg;

if( !hi.openKeyboard( 0 ) ) me.exit();
<<< "keyboard ready", "" >>>;

.5 => float gain;

PercFlut flute => dac;
0 => flute.gain;

while( true )
{
    hi => now;
    while( hi.recv( msg ) )
    {
        if( msg.is_button_down() )
        {
            std.mtof( msg.which + 30 ) => flute.freq;
            gain => flute.gain;
        }
        else
        {
            0 => flute.gain;
        }
            
        
    }
}


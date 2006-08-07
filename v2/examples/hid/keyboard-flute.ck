// HID
HidIn hi;
HidMsg msg;

// open keyboard
if( !hi.openKeyboard( 0 ) ) me.exit();
<<< "keyboard ready", "" >>>;

// patch
BeeThree flute => JCRev r => Echo e => Echo e2 => dac;
r => dac;

// set delays
240::ms => e.max => e.delay;
480::ms => e2.max => e2.delay;
// set gains
.6 => e.gain;
.3 => e2.gain;
.05 => r.mix;
0 => flute.gain;

// infinite event loop
while( true )
{
    // wait for event
    hi => now;

    // get message
    while( hi.recv( msg ) )
    {
        // check
        if( msg.is_button_down() )
        {
            std.mtof( msg.which + 45 ) => float freq;
            if( freq > 20000 ) continue;

            freq => flute.freq;
            .5 => flute.gain;
            1 => flute.noteOn;

            80::ms => now;
        }
        else
        {
            0 => flute.noteOff;
        }
    }
}

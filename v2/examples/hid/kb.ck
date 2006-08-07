HidIn hi;
HidMsg msg;

// open keyboard
if( !hi.openKeyboard( 0 ) ) me.exit();
<<< "keyboard ready", "" >>>;

// infinite event loop
while( true )
{
    // wait on event
    hi => now;

    // get one or more messages
    while( hi.recv( msg ) )
    {
        // check for action type
        if( msg.is_button_down() )
        {
            <<< "down:", msg.which >>>;
        }
        else
        {
            // <<< "up:", msg.which >>>;
        }
    }
}

HidIn hi;
HidMsg msg;

// open keyboard
if( !hi.openKeyboard( 0 ) ) me.exit();
<<< "keyboard '", hi.name(), "' ready" >>>;

// infinite event loop
while( true )
{
    // wait on event
    hi => now;

    // get one or more messages
    while( hi.recv( msg ) )
    {
        // check for action type
        if( msg.isButtonDown() )
        {
            <<< "down:", msg.which, msg.key, msg.ascii >>>;
        }
        else
        {
            // <<< "up:", msg.which, msg.key, msg.ascii >>>;
        }
    }
}

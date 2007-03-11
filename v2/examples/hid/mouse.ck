// make HidIn and HidMsg
HidIn hi;
HidMsg msg;

// open mouse 0, exit on fail
if( !hi.openMouse( Std.atoi(me.getArg(0)) ) ) me.exit();
<<< "mouse '", hi.name(), "' ready" >>>;

// infinite event loop
while( true )
{
    // wait on HidIn as event
    hi => now;

    // messages received
    while( hi.recv( msg ) )
    {
        // mouse motion
        if( msg.isMouseMotion() )
        {
            if( msg.deltaX )
            {
                <<< "mouse motion:", msg.deltaX, "on x-axis" >>>;
            }
            
            if( msg.deltaY )
            {
                <<< "mouse motion:", msg.deltaY, "on y-axis" >>>;
            }
        }
        
        // mouse button down
        else if( msg.isButtonDown() )
        {
            <<< "mouse button", msg.which, "down" >>>;
        }
        
        // mouse button up
        else if( msg.isButtonUp() )
        {
            <<< "mouse button", msg.which, "up" >>>;
        }
        
        // mouse wheel motion
        else if( msg.isWheelMotion() )
        {
            if( msg.deltaX )
            {
                <<< "mouse wheel:", msg.deltaX, "on x-axis" >>>;
            }
            
            if( msg.deltaY )
            {
                <<< "mouse wheel:", msg.deltaY, "on y-axis" >>>;
            }
        }
    }
}

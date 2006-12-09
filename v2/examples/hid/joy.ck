// make HidIn and HidMsg
HidIn hi;
HidMsg msg;

// open joystick 0, exit on fail
if( !hi.openJoystick( 0 ) ) me.exit();

<<< "joystick '", hi.name(), "' ready" >>>;

// infinite event loop
while( true )
{
    // wait on HidIn as event
    hi => now;

    // messages received
    while( hi.recv( msg ) )
    {
        // joystick axis motion
        if( msg.isAxisMotion() )
        {
            <<< "joystick axis", msg.which, ":", msg.axisPosition >>>;
        }
        
        // joystick button down
        else if( msg.isButtonDown() )
        {
            <<< "joystick button", msg.which, "down" >>>;
        }
        
        // joystick button up
        else if( msg.isButtonUp() )
        {
            <<< "joystick button", msg.which, "up" >>>;
        }
        
        // joystick hat/POV switch/d-pad motion
        else if( msg.isHatMotion() )
        {
            <<< "joystick hat", msg.which, ":", msg.idata >>>;
        }
    }
}

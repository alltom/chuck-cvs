HidIn hi;
HidMsg msg;

hi.openJoystick( 0 );

while( true )
{
    hi => now;
    while( hi.recv( msg ) )
    {
        <<< msg.type, msg.which, msg.idata, msg.fdata >>>;
    }
}

HidIn hi;
HidMsg msg;

if( !hi.openTiltSensor() )
{
    <<< "tilt sensor unavailable", "" >>>;
    me.exit();
}

<<< "tilt sensor ready", "" >>>;

while( true )
{
    hi.read( 0, 0, msg );
    
    <<< msg.x, msg.y, msg.z >>>;
    
    1::second => now;
}

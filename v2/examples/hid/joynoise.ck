HidIn hi;
HidMsg msg;

noise n => biquad f => Envelope e => pan2 p => dac;
// set biquad pole radius
.99 => f.prad;
// set biquad gain
.05 => f.gain;
// set equal zeros
1 => f.eqzs;
// our float
0.0 => float t;

e => Echo echo => p;
e => Echo echo2 => p;
e => Echo echo3 => p;
echo.delay() * 2 => echo2.max => echo2.delay;
echo.delay() * 3 => echo3.max => echo3.delay;
echo.gain() * .5 => echo2.gain;
echo2.gain() * .5 => echo3.gain;
.5 => n.gain;
440 => f.pfreq;
0 => float value;
1 => float factor;

// open joystick
if( !hi.openJoystick( 0 ) ) me.exit();
<<< "joystick ready...", "" >>>;

// infinite time loop
while( true )
{
    // wait on event
    hi => now;
    // recv message
    while( hi.recv( msg ) )
    {
        // axis 0
        if( msg.type == 0 && msg.which == 0 )
        { msg.fdata => p.pan; }

        // axis 1
        if( msg.type == 0 && msg.which == 1 )
        { -msg.fdata => value; set( value, factor ); }

        // button down (any)
        if( msg.type == 1 /*&& msg.which == 7*/ )
        { e.keyOn(); msg.which + 1 => factor; set( value, factor ); }

        // button up (any)
        if( msg.type == 2 /*&& msg.which == 7*/ )
        { e.keyOff(); }
    }
}

fun void set( float value, float factor )
{
    (value * 440 + 480) * factor => f.pfreq;
}

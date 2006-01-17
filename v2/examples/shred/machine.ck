// on-the-fly shred management

// this is kind of like using the OTF commands 
//     add / remove / replace etc.
// except this is done from code and also can be timed.
// (this is also a hack for including other files)

// infinite time loop 
while( true )
{
    // the path is relative to where 'chuck' is invoked
    machine.add( "../basic/moe.ck" ) => int moe;

    // the operation is sample synchronous
    500::ms => now;

    // replace
    machine.replace( moe, "../basic/wind.ck" ) => int wind;

    500::ms => now;

    // remove
    machine.remove( wind );
}


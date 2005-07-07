event e;

fun int hi( event e )
{
    e => now;
    <<<"success">>>;
}

spork ~ hi( e );
spork ~ hi( e );
spork ~ hi( e );
spork ~ hi( e );

1::second => now;

e.broadcast();

1::ms => now;

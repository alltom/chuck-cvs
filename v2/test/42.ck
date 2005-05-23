event e;

fun int hi( event e )
{
    e => now;
    <<<"hi">>>;
}

spork ~ hi( e );
spork ~ hi( e );
spork ~ hi( e );
spork ~ hi( e );

1::second => now;

e.broadcast();

while( true )
{
    1::second => now;
}

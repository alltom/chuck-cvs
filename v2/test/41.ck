event e;

fun int hi( event e )
{
    e => now;
    <<<"hi">>>;
}

spork ~ hi( e );

1::second => now;

e.signal();

while( true )
{
    1::second => now;
}

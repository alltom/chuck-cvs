event e;

fun int hi( event e )
{
    e => now;
    <<<"success">>>;
}

spork ~ hi( e );

1::ms => now;

e.signal();

1::samp => now;

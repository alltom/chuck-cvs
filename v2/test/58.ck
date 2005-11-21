Skot skot;

while( true )
{
    // prompt
    skot.prompt();
    // wait for it
    skot => now;
    // get the results
    while( skot.more() )
    {
        <<< skot.getLine() >>>;
    }
}

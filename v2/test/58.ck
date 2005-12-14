Skot skot;

while( true )
{
    // prompt and wait
    skot.prompt("enter data:") => now;
    // get the results
    while( skot.more() )
    {
        <<< skot.getLine() >>>;
    }
}

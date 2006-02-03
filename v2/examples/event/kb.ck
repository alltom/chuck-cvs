// the event
KBHit kb;

// time-loop
while( true )
{
    kb => now;

    while( kb.more() )
    {
        <<< "ascii: ", kb.getchar() >>>;
    }
}


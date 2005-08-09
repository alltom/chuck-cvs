
// number of total columns
9 => int total;

0 => int c;

// do the set
machine.add( "set.ck" );

// spork many
while( c < total )
{
    c++;
    // set column
    std.setenv( "VCR_COLUMN", std.itoa( c ) );
    // spork
    machine.add( "yay.ck" );
}

// three strings
"hello" => string foo;
"hello" => string bar;
"there" => string geh;

// makeshift assert
fun void assert( int condition, string text )
{
    if( !condition )
    {
        <<< "assertion failed: ", text >>>;
        me.exit();
    }
}

// equality
assert( foo == foo, "1" );
assert( foo == bar, "2" );
assert( "abc" == "abc", "3" );
assert( "hello" == foo, "4" );

assert( foo != geh, "5" );
assert( "x" != "y", "6" );
assert( foo != "there", "7" );

// lt
assert( foo < geh, "8" );
assert( foo < "hello!", "9" );
assert( foo <= foo, "10" );
assert( foo <= "there", "11" );

// gt
assert( foo > "abc", "12" );
assert( foo > "b", "13" );
assert( foo >= foo, "14" );
assert( foo >= bar, "15" );

<<< "success" >>>;

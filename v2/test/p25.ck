// pass by value test - should NOT hang on infinite loop

fun void fake_equals( float arg1, float arg2 )
{
	arg1 => arg2;
}

2 => float x;
3 => float y;

fake_equals( x, y );

if ( x == y ) { 
	while ( true );
}




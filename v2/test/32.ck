
class X
{
    float f;
    fun float foo( float a, float b )
    { return a + b; }
}

X x;
if( (( 1, 2 ) => x.foo) == 3 )
    <<<"success">>>;


class X
{
    int i;

    public void foo( int a )
    {
        a => i;
    }
}

X x;
x.foo( 2 );

if( x.i == 2 ) <<<"success">>>;


// declare class with a member field and a member function
class X
{
    int i;

    public void foo( int a )
    {
        a => i;
    }
}

// instantiate a X
X x;
x.foo( 2 );

if( x.i == 2 ) <<<"success">>>;

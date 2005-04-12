class X
{
    float x;
}

class Y
{
    X x;
    fun X foo() { return x; };
}

Y y;
12.2 => y.x.x;

if( y.foo().x == 12.2 ) <<<"success">>>;

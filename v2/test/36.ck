
class X
{
    fun void foo( int a )
    { <<<a>>>; }

    fun void foo( int a, int b )
    { <<<a+b>>>; }
}

class Y extends X
{
    fun void foo( int a )
    { <<<a+1>>>; }

    fun void foo( int a, int b )
    { <<<a+b+1>>>; }
}

class Z extends Y
{
    fun void foo( int a, int b )
    { <<<a+b+2>>>;}
}

Z x;
x.foo(3,5);


class X
{
    fun void foo( int a )
    { <<<a>>>; }

    fun void foo( int a, int b )
    { <<<a+5>>>; }
}

//class Y extends X
//{
//    fun void foo( int a )
//    { <<<a+1>>>; }
//}

X x;
x.foo(3,5);

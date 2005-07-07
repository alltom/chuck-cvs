
class X
{
    fun void foo( int a )
    { <<<a>>>; }

    fun void foo( int a, int b )
    { <<<"success">>>; }
}

//class Y extends X
//{
//    fun void foo( int a )
//    { <<<a+1>>>; }
//}

X x;
x.foo(3,5);

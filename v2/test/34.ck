// function overloading

// declare class
class X
{
    // delcare foo
    fun void foo( int a )
    { <<<a>>>; }

    // delcare foo with different arguments
    fun void foo( int a, int b )
    { <<<"success">>>; }
}

//class Y extends X
//{
//    fun void foo( int a )
//    { <<<a+1>>>; }
//}

// instantiate a X
X x;
// call the right one, hopefully
x.foo(3,5);

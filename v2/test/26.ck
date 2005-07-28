// delcare class
class X
{
    int x;
}

// declare another class
class Y
{
    // declare a X as a member of Y
    X x;
    // return x through a function
    fun X foo() { return x; };
}

// instantiate a Y
Y y;
// assign to y's x's field x
1 => y.x.x;

// get x through the function and test the value
if( y.foo().x != 1 ) <<<"fail">>>;
else <<<"success">>>;

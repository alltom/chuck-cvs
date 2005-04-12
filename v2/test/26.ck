class X
{
    int x;
}

class Y
{
    X x;
    fun X foo() { return x; };
}

Y y;
1 => y.x.x;

if( y.foo().x != 1 ) { 
	<<<"fail">>>;
}
else { 
	<<<"success">>>;
}

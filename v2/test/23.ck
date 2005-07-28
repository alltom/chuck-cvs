// declare class X
class X
{
    int i;
}

// declare class Y as subclass of X
class Y extends X
{
    int j;
}

// instantiate a Y
Y y;
10 => y.i;

if( y.i == 10 && y.j != 0 ) { 
	<<<"test failed">>>;
}
else { 
	<<<"success">>>;
}

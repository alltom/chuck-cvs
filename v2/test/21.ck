
class X
{
    float f;
    int i;
}

class Y
{
    X x;
}

//fun void foo( Y x )
//{
//    10.0 => x.x.f;
//    1 => x.x.i;
//}

Y x;
//foo( x );

x.x @=> X @ y;

//if( y.f != 10.0 ) while( true );
//if( y.i != 1 ) while( true );

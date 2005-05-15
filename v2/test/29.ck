class X
{
    1 => int x;

//	fun void foo()
//	{
//	    2 => x;
//	}

    fun void setTestID( int id )
    {   <<< id + 1 >>>;
    }
}

X x;
x.setTestID( 3 );
x.testStatic( 2 );

//<<<x.getTestID()>>>;
Machine.status();
//<<<Math.sin( 1.0 )>>>;
//<<<Std.rand2f( 30.0, 1000.0 )>>>;

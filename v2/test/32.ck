
Object xs[10];
3 => xs[9].setTestID;
<<<xs[9].getTestID()>>>;
<<<xs[9].m_testID>>>;

class X
{
    float f;
    fun float foo( float a, float b )
    { return a + b; }
}

X x;
<<<( 1, 2 ) => x.foo>>>;

10 => xs[1].our_testID;
<<<xs[2].our_testID>>>;
<<<Object.our_testID>>>;

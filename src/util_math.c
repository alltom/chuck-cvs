//a mini-compatability library for 
//math functions that might be missing 
//on different systems.

#ifdef WIN32

double round( double a )
{
	double off = a - (long)a;
	if( off > .5 ) return (double)(long)a + 1.0;
	return (double)(long)a;
}

double trunc( double a )
{
	return (double)(long)a;
}

double remainder( int a, int b )
{
	int div = a/b;
	return a - b*div;
}

#endif

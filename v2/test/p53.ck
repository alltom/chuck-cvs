// implicit casts between int and float
// are we really this strict?


fun void takesInts ( int argI ) { 
	argI => int localI;
}

fun void takesFloats ( float argF ) { 
	argF => float localF;
}


takesFloats( 4 );
takesInts(5.0);

takesFloats(a);
takesInts(x);

1.0 => int a;
2 => float x;

y => int b;
a => float y;

5 * x => int c;
x * 5 => int d;
3.0 * x => int e;
x * 3.0 => int f;

3 * a => float m;
a * 3 => float n;
5.0 * a => float o;
a * 5.0 => float p;

 
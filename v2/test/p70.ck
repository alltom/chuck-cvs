// Point class

class Point { 
	float p[3];
	fun void set ( float x, float y, float z ) { 
		x => p[0];
		y => p[1];
		z => p[2];
	}
	fun void setA ( float[] np ) { 
		for ( 0 => int i; i < 3; i +=> i ) np[i] => p[i];
	}
	fun Point sum ( Point b ) { 
		Point ret;
		for ( 0 => int i; i < 3 ; 1 +=> i )  p[i] + b.p[i] => ret.p[i];
		return ret;
	}
}

Point p1;
Point p2;
p1.set( 0, 1, 2 );
p2.set( 5, 6, 7 );
p1.sum(p2) => Point p3;

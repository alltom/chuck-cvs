
class Note { 

	float freq;
	float vel;
	dur length;
	fun void set ( float note, float vl, dur ln ) { 
		note => freq;
		vl => vel;
		ln => length;
	}

	fun void playMe ( Mandolin m ) { 
		std.mtof( freq ) => m.freq;
		vel => m.pluck;
		length => now;
	}
	fun void playAlt( Mandolin m, float noff, float vmul, float lmul ) { 
		std.mtof( freq + noff ) => m.freq;
		vmul * vel => m.pluck;
		lmul * length => now;
	}
}

12 => int seqn;
Note sequence[seqn];
int order[seqn];
[0,2,4,7,9, 12, 14, 16, 19, 21] @=> int scale[];
10 => int nscale;
[0.25::second, 0.125::second, 0.125::second,  0.375::second] @=> dur times[];
4 => int ntimes;

//fun initialize ( Note sequence, int n ) { 

for ( 0 => int i; i < seqn ; i++ ) { 
	i => order[i];		
	55 + scale[std.rand2(0, nscale - 1)] => int note;
	times[std.rand2(0, ntimes - 1)] => dur mydur;
	std.rand2f( 0.75, 0.9 ) => float vel; 
	sequence[i].set( note, vel, mydur );
}

fun void swap( ) { 
	std.rand2(0,seqn-1) => int a;
	( a + std.rand2(1,seqn-1) ) % seqn => int b;
	order[a] => int tmp;
	order[b] => order[a];
	tmp => order[a];
}

Mandolin clar => dac;

while ( true ) { 

	for ( 0 => int j ; j < seqn; j++ ) { 
		sequence[order[j]].playMe(clar);
		sequence[order[j]].playAlt( clar, 7, 0.33, 0.5 );
	}
	for ( std.rand2(0,2) => int j ; j > 0 ; j-- ) { 
		swap();
	}


}



class Player { 
	fun void noteOn ( float note, float vel ) {}
	fun void noteOff ( float vel ) {}
}

class Note { 

	float note;
	float vel;
	dur length;
	fun void set ( float nt, float vl, dur ln ) { 
		nt => note;
		vl => vel;
		ln => length;
	}

	fun void playOn ( Player p) { 
		if ( note > 0 ) { 
			p.noteOn( note , vel );
		}
	}

	fun void playOnAlt( Player p, float noff, float vmul ) { 
		p.noteOn( note+noff, vel*vmul );
	}
}


class MandPlayer extends Player { 
	Mandolin m;
	m => dac;
	fun void noteOn ( float note, float vel ) { 
		std.mtof ( note ) => m.freq;
		vel => m.pluck;
	}
}

class ClarPlayer extends Player { 
	Clarinet c;
	c => dac;
	fun void noteOn ( float note, float vel ) { 
		std.mtof ( note ) => c.freq;
		vel => c.startBlowing;
	}
	fun void noteOff ( float vel ) { 
		vel => c.stopBlowing;
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
fun void newsequence() { 
	for ( 0 => int i; i < seqn ; i++ ) { 
		i => order[i];		
		55 + scale[std.rand2(0, nscale - 1)] => int note;
		times[std.rand2(0, ntimes - 1)] => dur mydur;
		std.rand2f( 0.75, 0.9 ) => float vel; 
		sequence[i].set( note, vel, mydur );
	}
}

fun void swap( ) { 
	std.rand2(0,seqn-1) => int a;
	( a + std.rand2(1,seqn-1) ) % seqn => int b;
	order[a] => int tmp;
	order[b] => order[a];
	tmp => order[a];
}

MandPlayer mand;
ClarPlayer clar;

newsequence();

while ( true ) { 

	for ( 0 => int j ; j < seqn; j++ ) { 
		sequence[order[j]] @=> Note cur;
		cur.playOn ( mand );
		cur.playOnAlt ( clar, 12 , 0.7 );
		cur.length => now;
		cur.playOnAlt( mand, 7, 0.33);
		0.5 * cur.length => now;
	}
	for ( std.rand2(0,2) => int j ; j > 0 ; j-- ) { 
		swap();
	}
	if ( std.rand2(0, 10) > 8 ) { 
		newsequence();
	}


}


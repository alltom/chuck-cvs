"gl.ckx" => (:gl:);
"glu.ckx" => (:glu:);
"gluck.ckx" => (:gluck:);

// walls
-1.0 => float l;
1.0 => float r;
-1.0 => float b;
1.0 => float t;

l => float wx1;
b => float wy1;

r => float wx2;
b => float wy2;

r => float wx3;
t => float wy3;

l => float wx4;
t => float wy4;

// source pos
0.5 => float srcx;
0.5 => float srcy;

// source animation
0.8 => float srcrad;
1.0 => float srcfreq;

// receive animation
0.5 => float recvrad;
1.0 => float recvfreq;


// recv ( dac ) pos
-0.25 => float recvx;
-0.25 => float recvy;

// audio constants
50.0 => float speedofsound;
0.95 => float reflectance;


// tmp reflection point
0.0 => float rx;
0.0 => float ry;


// saved reflection points
0.0 => float rxl;
0.0 => float ryl;
0.0 => float rxr;
0.0 => float ryr;
0.0 => float rxt;
0.0 => float ryt;
0.0 => float rxb;
0.0 => float ryb;

// saved mirror points?



// gluck pile


fun void draw() { 
	gl.Clear ( gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT );
	gl.Color4f( 0.0, 1.0, 0.0 ,0.5 );
	
	gl.LineWidth ( 1.0 );


 	gl.Begin(gl.LINE_LOOP);
	gl.Vertex2f ( wx1, wy1 );
	gl.Vertex2f ( wx2, wy2 );
	gl.Vertex2f ( wx3, wy3 );
	gl.Vertex2f ( wx4, wy4 );
	gl.End();


	gl.Begin(gl.LINES);

	gl.Color4f ( 1.0, 0.0 ,1.0, 0.5 );
	gl.Vertex2f( srcx, srcy );
	gl.Vertex2f( recvx, recvy );

	gl.Color4f ( 1.0, 0.0, 0.5, 0.25 );
	gl.Vertex2f( srcx, srcy );
	gl.Vertex2f( rxl, ryl );
	gl.Vertex2f( srcx, srcy );
	gl.Vertex2f( rxb, ryb );
	gl.Vertex2f( srcx, srcy );
	gl.Vertex2f( rxr, ryr );
	gl.Vertex2f( srcx, srcy );
	gl.Vertex2f( rxt, ryt );

	gl.Color4f ( 0.5, 0.0, 1.0, 0.25 );
	gl.Vertex2f( rxl, ryl );
	gl.Vertex2f( recvx, recvy );
	gl.Vertex2f( rxb, ryb );
	gl.Vertex2f( recvx, recvy );
	gl.Vertex2f( rxr, ryr );
	gl.Vertex2f( recvx, recvy );
	gl.Vertex2f( rxt, ryt );
	gl.Vertex2f( recvx, recvy );
	gl.End();

	gl.PointSize ( 8.0 );
	gl.Begin(gl.POINTS);
	gl.Color4f( 1.0, 0.0, 0.0, 1.0 );
	gl.Vertex2f ( srcx, srcy );
	gl.Color4f( 0.0, 0.0, 1.0, 1.0 );
	gl.Vertex2f ( recvx, recvy );
	gl.End();


	gl.Flush();
	gluck.SwapBuffers();
}


fun float dlen ( float x, float y ) { 
	return math.sqrt (x * x + y * y);
}

fun float dist ( float x, float y, float x1, float y1 ) { 
	return dlen ( x-x1, y-y1 );
}


fun float dot ( float x, float y, float xp, float yp ) { 
	return x*xp + y*yp;
}


//
//                    t
//       ----------------------
//		|                d     |
//   l  |                      |  r
//      |   s                  |
//      |_______o______________|
//                    b
//          s'
//                      
//      


fun void reflectwall( float w1x, float w1y, float w2x, float w2y ) { 

	//set up values for wall
	w2x - w1x => float wdx;
	w2y - w1y => float wdy;
	-wdy / dlen ( wdx, wdy ) => float wnx;
	wdx / dlen ( wdx, wdy ) => float wny;
 	- ( wnx * w1x + wny * w1y ) => float wd;

	// project vect ( wall to d ) to the wall normal
	// then subtract twice that from the point.
	// to find the reflection

	srcx - w1x => float sdx;
	srcy - w1y => float sdy;
	dot ( sdx, sdy , wnx, wny ) => float p_n_dp;
	p_n_dp * wnx => float p_sdx;
	p_n_dp * wny => float p_sdy;
	srcx - 2.0 * p_sdx => float mirx;
	srcy - 2.0 * p_sdy => float miry;

	//find intersection of ray from recv pt. to reflection point
	//with the wall
	//store in global rx, ry

	mirx - recvx => float drefx;
	miry - recvy => float drefy;	
        - ( dot( wnx,wny , recvx,recvy ) + wd ) / dot( wnx, wny, drefx, drefy ) => float twall;
	recvx + drefx * twall => rx;
	recvy + drefy * twall => ry;
//	w1x => rx;
//	w1y => ry;

}

fun void setdistdelay ( DelayL del, gain g, float x1, float y1, float x2, float y2 ) { 
	dlen ( x2 - x1 , y2 - y1 ) => float len;
	len / speedofsound => float deltime;
	1.0 / ( len*len ) => float atten;
	1::second * deltime=> del.delay;
	atten => g.gain;
//	"distdelay" => stdout;
//	deltime => stdout;
//	atten => stdout;
}

fun void setecho ( DelayL del, gain g, float len, float baseatten ) { 
	len / speedofsound => float deltime;
	1.0 / ( len*len ) => float atten;
	1::second * deltime=> del.delay;
	baseatten * atten => g.gain;
//	"distdelay" => stdout;
//	deltime => stdout;
//	atten => stdout;
}

gain src => DelayL d_direct => gain g_direct => dac;

//4 1st order reflections
src => DelayL s_t => gain g_t => dac ; 
src => DelayL s_b => gain g_b => dac ; 
src => DelayL s_l => gain g_l => dac ; 
src => DelayL s_r => gain g_r => dac ; 

10::second => d_direct.max;

10::second => s_t.max;
10::second => s_b.max;
10::second => s_l.max;
10::second => s_r.max;

fun void refreshenv () { 
//	"refresh" => stdout;
	setdistdelay ( d_direct, g_direct, srcx, srcy, recvx, recvy );

	//wall b
	reflectwall ( wx1 , wy1 , wx2, wy2 ); 
	rx => rxb;
	ry => ryb;
	setecho ( s_b, g_b, dist( srcx, srcy, rx, ry ) + dist ( rx, ry, recvx, recvy ), reflectance );
	
	//wall t
	reflectwall ( wx3, wy3 , wx4, wy4 ); 
	rx => rxt;
	ry => ryt;
	setecho ( s_t, g_t, dist( srcx, srcy, rx, ry ) + dist ( rx, ry, recvx, recvy ), reflectance );

	//wall l
	reflectwall ( wx4 , wy4 , wx1, wy1 ); 
	rx => rxl;
	ry => ryl;
	setecho ( s_l, g_l, dist( srcx, srcy, rx, ry ) + dist ( rx, ry, recvx, recvy ), reflectance );

	//wall r
	reflectwall ( wx2 , wy2 , wx3, wy3 ); 
	rx => rxr;
	ry => ryr;
	setecho ( s_r, g_r, dist( srcx, srcy, rx, ry ) + dist ( rx, ry, recvx, recvy ), reflectance );

}


refreshenv();

fun void keycode(uint key) { 
	if ( key == gluck.KEY_w ) { srcfreq * math.pow ( 2.0, 1.0/12.0) => srcfreq; }
	if ( key == gluck.KEY_s ) { srcfreq * 1.0 / math.pow ( 2.0, 1.0/12.0) => srcfreq; }
	if ( key == gluck.KEY_e ) { srcfreq * -1.0  => srcfreq; }
	if ( key == gluck.KEY_d ) { srcrad + 0.01  => srcrad; }
	if ( key == gluck.KEY_a ) { srcrad - 0.01  => srcrad; }

	if ( key == gluck.KEY_i ) { recvfreq * math.pow ( 2.0, 1.0/12.0) => recvfreq; }
	if ( key == gluck.KEY_k ) { recvfreq * 1.0 / math.pow ( 2.0, 1.0/12.0) => recvfreq; }
	if ( key == gluck.KEY_o ) { recvfreq * -1.0  => recvfreq; }
	if ( key == gluck.KEY_l ) { recvrad + 0.01  => recvrad; }
	if ( key == gluck.KEY_j ) { recvrad - 0.01  => recvrad; }

}

fun void gluckEvents() { 
	while ( true ) { 
		while ( gluck.HasEvents() ) { 
			gluck.GetNextEvent() => int curid;
			gluck.GetEventType(curid) => uint curtype;
		 	if ( curtype == gluck.EVENT_KEY ) { //keyboard
				gluck.GetEventKey(curid) => uint k;
				keycode(k);
			}
		}
		30::ms => now;
	}
}


fun void gluckStartup ( ) { 

	gluck.Init();
	gluck.InitSizedWindow( "echobox", 80,80, 640, 640);
	gluck.InitCallbacks( 0,0,1 );
	
	spork ~ gluckEvents();
	
	gl.MatrixMode(gl.PROJECTION); 
	gl.LoadIdentity();
	gl.Ortho(-2.0, 2.0, -2.0 , 2.0, -4.0 , 4.0 );

	gl.MatrixMode(gl.MODELVIEW); 
	gl.LoadIdentity();

	gl.ClearColor ( 0.1 , 0.1, 0.1, 0.0 );
	gl.Enable(gl.LINE_SMOOTH);
	gl.Enable(gl.POINT_SMOOTH);
	gl.Enable(gl.POLYGON_SMOOTH);
	gl.Enable(gl.BLEND);
	gl.BlendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);
	gl.Disable(gl.DEPTH_TEST);


	while ( true ) { 
		gluck.MainLoopEvent();
		if ( gluck.NeedDraw() ) draw();
		gluck.PostRedisplay();
		33::ms => now;
	}

}

//THE FUN PART...

fun void fiddlebox () { 
	phasor sc => sinosc sx => blackhole;
	sc => sinosc sy => blackhole; 
	step bias => sx;
	
	phasor rc => sinosc rxo => blackhole;
	rc => sinosc ryo => blackhole; 
	bias => rxo;
	
	2 => sx.sync;
	2 => sy.sync;
	2 => rxo.sync;
	2 => ryo.sync;
	1.0  => sc.sfreq;
	1.0  => rc.sfreq;
	0.25 => bias.next;
	
	while ( true ) { 
		srcrad * sx.last => srcx;
		srcrad * sy.last => srcy;		
		recvrad * rxo.last => recvx;
		recvrad * ryo.last => recvy;		

		refreshenv();
		srcfreq => sc.sfreq;
		recvfreq => rc.sfreq;
		10::samp => now;
	}
}

impulse i ;// => src;
sinosc m => src;
220.0 => m.sfreq;
0.00 => m.gain;
sndbuf buf ;// => src;
"puzzler.au" => buf.read;
1.0 => buf.rate;
1 => buf.loop;
0.1 => buf.gain;
noise n => src;
0.02 => n.gain;
spork ~ gluckStartup();

spork ~ fiddlebox();
while ( true ) {
	0.3 => i.next; 
	0.25::second => now;
}




"gl.ckx" => (:gl:);
"glu.ckx" => (:glu:);
"gluck.ckx" => (:gluck:);

//walls
-1.0 => float l;
1.0 => float r;
-1.0 => float b;
1.0 => float t;

//source pos
0.5 => float srcx;
0.5 => float srcy;

//recv ( dac ) pos
-0.25 => float recvx;
-0.25 => float recvy;

//audio constants
50.0 => float speedofsound;
0.95 => float reflectance;


//tmp reflection point
0.0 => float rx;
0.0 => float ry;


//saved reflection points
0.0 => float rxl;
0.0 => float ryl;
0.0 => float rxr;
0.0 => float ryr;
0.0 => float rxt;
0.0 => float ryt;
0.0 => float rxb;
0.0 => float ryb;

//saved mirror points?



//gluck pile


fun void draw() { 
	gl.Clear ( gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT );
	gl.Color4f( 0.0, 1.0, 0.0 ,0.5 );
	
	gl.LineWidth ( 1.0 );


 	gl.Begin(gl.LINE_LOOP);
	gl.Vertex2f ( l, t );
	gl.Vertex2f ( r, t );
	gl.Vertex2f ( r, b );
	gl.Vertex2f ( l, b );
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

fun float dot ( float x, float y, float xp, float yp ) { 
	return x*xp + y*yp;
}


//
//                    t
//	 ----------------------
//	|                d     |
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

gain src => DelayL d_direct => gain g_direct => dac;

//4 1st order reflections
src => DelayL s_t => gain g_t => gain reft => DelayL t_d => gain t_r => dac;
src => DelayL s_b => gain g_b => gain refb => DelayL b_d => gain b_r => dac;
src => DelayL s_l => gain g_l => gain refl => DelayL l_d => gain l_r;// => dac;
src => DelayL s_r => gain g_r => gain refr => DelayL r_d => gain r_r;// => dac;


10::second => d_direct.max;

10::second => s_t.max;
10::second => s_b.max;
10::second => s_l.max;
10::second => s_r.max;

10::second => t_d.max;
10::second => b_d.max;
10::second => l_d.max;
10::second => r_d.max;

fun void refreshenv () { 
//	"refresh" => stdout;
	setdistdelay ( d_direct, g_direct, srcx, srcy, recvx, recvy );

	//wall b
	reflectwall ( l , b , r, b ); 
	rx => rxb;
	ry => ryb;
	setdistdelay ( s_b, g_b, srcx, srcy, rx, ry );
	reflectance => refb.gain;
	setdistdelay ( b_d, b_r, rx, ry, recvx, recvy );

	//wall t
	reflectwall ( r , t , l, t ); 
	rx => rxt;
	ry => ryt;
	setdistdelay ( s_t, g_t, srcx, srcy, rx, ry );
	reflectance => reft.gain;
	setdistdelay ( t_d, t_r, rx, ry, recvx, recvy );

	//wall l
//	reflectwall ( l , t , l, b ); 
	rx => rxl;
	ry => ryl;
	setdistdelay ( s_l, g_l, srcx, srcy, rx, ry );
	reflectance => refl.gain;
	setdistdelay ( l_d, l_r, rx, ry, recvx, recvy );

	//wall r
//	reflectwall ( r , b , r, t ); 
	rx => rxr;
	ry => ryr;
	setdistdelay ( s_r, g_r, srcx, srcy, rx, ry );
	reflectance => refr.gain;
	setdistdelay ( r_d, r_r, rx, ry, recvx, recvy );

}


refreshenv();



fun void gluckStartup ( ) { 

	gluck.Init();
	gluck.InitSizedWindow( "echobox", 80,80, 640, 640);
	gluck.InitCallbacks( 0,0,0 );
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
	sinosc s => blackhole;
	sinosc p => blackhole;
	0.5 => s.sfreq;
	0.5 => p.sfreq;
	0.25 => p.phase_offset;
	while ( true ) { 
		0.8 * s.last => srcx;
		0.8 * p.last => srcy;
		
//		l + std.rand2f(-0.001, 0.001 ) => l;
//		r + std.rand2f(-0.001, 0.001 ) => r;
//		b + std.rand2f(-0.001, 0.001 ) => b;
//		t + std.rand2f(-0.001, 0.001 ) => t;
		refreshenv();
		5::samp => now;
	}
}

impulse i => src;
sinosc m ;// => src;
440.0 => m.sfreq;
0.2 => m.gain;

spork ~ gluckStartup();

spork ~ fiddlebox();
while ( true ) {
	0.3 => i.next; 
	30::ms => now;
}




"gl.ckx" => (:gl:);
"glu.ckx" => (:glu:);
"gluck.ckx" => (:gluck:);

"namespaces loaded" => stdout;

gluck.Init();

"gluck init " => stdout;

gluck.InitCallbacks(1, 1, 1);
42 => uint placeholder;
gluck.InitBasicWindow(placeholder);

"gluck setup " => stdout;
0.0 => float rx;
0.0 => float ry;

sinosc a => dac;
440.0 => a.freq;
0.3 => a.gain;

5889 => uint dummy;

"audio setup" => stdout;

gl.MatrixMode(dummy); //projection
gl.LoadIdentity();
gl.Ortho(-1.0, 1.0, -1.0 , 1.0, -4.0 , 4.0 );

5888 => dummy;
gl.MatrixMode(dummy); //modelview
gl.LoadIdentity();

gl.ClearColor ( 0.0 , 0.0, 0.3, 0.0 );
0.0 => float tm;

"gl_setup" => stdout;



function void theeventloop() { 
	"the event loop!" => stdout;
	while ( gluck.HasEvents() ) { 
		gluck.GetNextEvent() => int id;
		gluck.GetEventX(id) => stdout;
		gluck.GetEventY(id) => stdout;
	}
}

"event func def" => stdout;
//chuck loop!

0.0 => float frac;
while ( true ) { 

	gluck.MainLoopEvent(); //...

	if ( gluck.NeedDraw() ) { 
		"needs draw!" => stdout;
	}
	if ( gluck.NeedReshape() != 0 ) { 
		"need reshape!" => stdout;
	}

	
//	if ( gluck.NeedDraw() ) {	


		16640 => dummy; //GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
		gl.Clear(dummy);
		std.abs ( 2.0 * ( -0.5 + tm - math.floor(tm)) ) => frac;
		
		gl.Color3f( frac , 1.0-frac ,0.0);
		gl.LineWidth(2.0);
		gl.PushMatrix();
		gl.Scalef  ( 1.1 + math.sin ( tm * 0.2 ), 1.1 + math.sin ( tm * 0.2 ), 1.1 + math.sin ( tm * 0.2 ) ); 
		gl.Rotatef ( tm * 45.0, 1.0, 0.0 , 0.0 );
		gl.Rotatef (tm * 60.0, 0.0, 1.0 , 0.0 );

		gluck.WireTeapot( 0.5 );

		gl.PopMatrix();
		gl.Flush();

		gluck.SwapBuffers();
//	}

	if ( gluck.NeedEvent() ) {
		theeventloop();
	}


	440.0 + 220.0 * frac => a.sfreq;
	tm + 0.033 => tm;
	math.sin ( tm ) => rx;
	math.cos ( tm * 3.0 ) => ry;

	gluck.PostRedisplay();
	33::ms => now;
}

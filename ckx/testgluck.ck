
"gl.ckx" => (:gl:);
"glu.ckx" => (:glu:);
"gluck.ckx" => (:gluck:);



gluck.Init();
gluck.InitCallbacks(1, 0 ,0);
42 => uint placeholder;
gluck.InitBasicWindow(placeholder);

0.0 => float rx;
0.0 => float ry;

sinosc a => dac;
440.0 => a.freq;
0.3 => a.gain;
5889 => uint dummy;
gl.MatrixMode(dummy); //projection
gl.LoadIdentity();
gl.Ortho(-1.0, 1.0, -1.0 , 1.0, -1.0 , 1.0 );

5888 => dummy;
gl.MatrixMode(dummy); //modelview
gl.LoadIdentity();

gl.ClearColor ( 0.0 , 0.3, 0.0, 0.0 );
0.0 => float tm;

function void thedrawloop () { 

		"the draw loop!" => stdout;
//		tm => stdout;
//		now => stdout;
		16640 => dummy; //GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
		gl.Clear(dummy);
		gl.Color3f(1.0, 0.0 ,0.0);
		gl.LineWidth(2.0);
		gl.PushMatrix();
		gl.Rotatef ( 45.0, 1.0, 0.0 , 0.0 );
		gl.Rotatef (tm * 60.0, 0.0, 1.0 , 0.0 );
		gluck.WireTeapot( 0.5 );
		gl.PopMatrix();
		gl.Flush();
		gluck.SwapBuffers();
		

}

function void theeventloop() { 
	"the event loop!" => stdout;
	while ( gluck.HasEvents() ) { 
		gluck.GetNextEvent() => int id;
		gluck.GetEventX(id) => stdout;
		gluck.GetEventY(id) => stdout;
	}
}

//chuck loop!

while ( true ) { 

	gluck.MainLoopEvent(); //...

	if ( gluck.NeedDraw() ) {		
		thedrawloop(); 
	}
	if ( gluck.NeedEvent() ) {
		theeventloop();
	}
	tm + 0.033 => tm;
	math.sin ( tm ) => rx;
	math.cos ( tm * 3.0 ) => ry;
	"chuck loop" => stdout;
	33::ms => now;
}

// morse code - gluck keyboard event demo 
// june 2004 Philip Davidson ( philipd@alumni.princeton.edu )
//
//
//

"gl.ckx" => (:gl:);
"glu.ckx" => (:glu:);
"gluck.ckx" => (:gluck:);

//our variables

Shakers shake1 => JCRev r => Echo a => Echo b => Echo c => dac;
Shakers shake2 => r;
TubeBell t => r;
0.2 => t.gain;
r => dac;


0.0 => float puckx;
0.0 => float pucky;

0.0 => float puckvx;
0.0 => float puckvy; 

0.0 => float puckax;
0.0 => float puckay;

0.02 => float puckrad;

0.0 => float  pad1y;
0.0 => float pad1vy;

0.0 => float pad2y; 
0.0 => float pad2vy;

0.03 => float padw;
0.3 => float padh;

0.1 => float padspace;

1.0 => float boardw;
1.0 => float boardh;

0 => int bouncex;
0 => int bouncey;

0 => int score1;
0 => int score2;

0 => int out1;
0 => int out2;

0 => int lastx;
0 => int lasty;

1 => int running;

function void keycode ( uint k ) { 
	k => stdout;

	if ( k == 113 ) { 
		0 => running;
	}	

	if ( k == 119 ) { 
		math.max ( 0.2, math.min ( 1.6 , pad1vy + 0.2) ) => pad1vy;
	}
	if ( k == 115 ) { 
		0.0 => pad1vy;
	}
	if ( k == 120 ) { 
		math.min ( -0.2 , math.max (-1.6 , pad1vy - 0.2) ) => pad1vy;
	}

	if ( k == 105 ) { 
		math.max ( 0.2, math.min ( 1.6 , pad2vy + 0.2) ) => pad2vy; 
	}
	if ( k == 106 ) { 
		0.0 => pad2vy;
	}
	if ( k == 110 ) { 
		math.min ( -0.2, math.max (-1.6 , pad2vy - 0.2) ) => pad2vy;
	}
	if ( k == 32 ) { 
		std.rand2f( 0.0, 100.0 ) => float deg;
		std.rand2f( 1.0, 2.0 ) => float mag;
		mag * math.cos(deg) => puckvx;
		mag * math.sin(deg) => puckvy;
//		0.0 => puckx;
//		0.0 => pucky;
		std.rand2(0,22) => shake1.which;
		std.rand2(0,22) => shake2.which;
		puckvx => stdout;
		puckvy => stdout;
	}
}

function void motion ( int x, int y ) { 
	x => lastx;
	y => lasty;
} 


function void drag ( int x, int y ) { 
	x - lastx => int dx;
	y - lasty => int dy;
	if ( x > 300 ) { 
		-0.6 * (float)dy => pad2vy;
	}
	else { 
		-0.6 * (float)dy => pad1vy;
	}	
}


function void theeventpoll() { 
	while ( gluck.HasEvents() ) { 

		gluck.GetNextEvent() => int curid;
		gluck.GetEventType(curid) => uint curtype;

	 	if ( curtype == gluck.EVENT_KEY ) { //keyboard
			gluck.GetEventKey(curid) => uint k;
			keycode(k);
		}
	 	if ( curtype == gluck.EVENT_PMOTION ) { //keyboard
			gluck.GetEventX(curid) => int x;
			gluck.GetEventY(curid) => int y;
			motion(x,y);
		}
	        if ( curtype == gluck.EVENT_MOTION ) { //keyboard
			gluck.GetEventX(curid) => int x;
			gluck.GetEventY(curid) => int y;
			drag(x,y);
			motion(x,y);
		}
	}
}

function void thedrawloop() { 

		gl.ClearColor( 0.4, 0.4, 0.4 , 0.0);
		gl.Clear( gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT );

		gl.LineWidth(3.0);
		gl.Color4f ( 0.0, 1.0, 0.0, 0.5 ) ;

		gl.Begin(gl.LINE_LOOP);
		gl.Vertex2f(-boardw, -boardh);
		gl.Vertex2f( boardw, -boardh);
		gl.Vertex2f( boardw,  boardh);
		gl.Vertex2f(-boardw,  boardh);
		gl.End();

		gl.PushMatrix();
		gl.Translatef ( -boardw + padspace, pad1y, 0.0 );
		gl.Begin(gl.QUADS );
		gl.Vertex3f ( -padw, -padh, 0.0 );
		gl.Vertex3f (  padw, -padh, 0.0 );
		gl.Vertex3f (  padw,  padh, 0.0 );
		gl.Vertex3f ( -padw,  padh, 0.0 );
		gl.End();
		gl.PopMatrix();

		gl.PushMatrix();
		gl.Translatef (  boardw - padspace, pad2y, 0.0 );
		gl.Begin(gl.QUADS );
		gl.Vertex3f ( -padw, -padh, 0.0 );
		gl.Vertex3f (  padw, -padh, 0.0 );
		gl.Vertex3f (  padw,  padh, 0.0 );
		gl.Vertex3f ( -padw,  padh, 0.0 );
		gl.End();
		gl.PopMatrix();

		gl.PushMatrix();
		gl.Translatef ( puckx ,  pucky , 0.1);
		gl.Begin(gl.QUADS) ;
		gl.Vertex2f ( -puckrad, -puckrad );
		gl.Vertex2f (  puckrad, -puckrad );
		gl.Vertex2f (  puckrad,  puckrad );
		gl.Vertex2f ( -puckrad,  puckrad );
		gl.End();
		gl.PopMatrix();

		gl.Flush();
		gluck.SwapBuffers();
}

function void audioshred( ) { 

 
}

function void sndtrigger ( int which ) { 

	if ( which == 1 ) { 
		1.5 => shake1.noteOn;
	}
	else if ( which == 2 ) { 
		1.5 => shake2.noteOn;
	}
	else if ( which == 3 ) { 
		440.0 => t.freq;
//		math.max ( 0.0, math.min ( 1.0 , std.abs( puckvx * 0.3))) => m.stringDetune;
		0.9  + math.max ( 0.0, math.min ( 1.0 , std.abs( puckvy * 0.3 ))) => t.noteOn;
	}
}

0.001 => float dt;
0.0 => float p1f;
0.0 => float p2f;

function void testcollisions() { 
	
	//pad1;
	if ( pad1y + padh > boardh ) { 
		pad1vy * -1.0 => pad1vy;
		boardh + (  boardh - ( pad1y + padh ) ) - padh => pad1y;
	}
	if ( pad1y - padh < -boardh ) { 
		pad1vy * -1.0 => pad1vy;
		-boardh + ( -boardh - (pad1y-padh) ) + padh  => pad1y;
	}


	//pad2;
	if ( pad2y + padh > boardh ) { 
		pad2vy * -1.0 => pad2vy;
		boardh + (  boardh - ( pad2y + padh ) ) - padh => pad2y;
	}
	if ( pad2y - padh < -boardh ) { 
		pad2vy * -1.0 => pad2vy;
		-boardh + ( -boardh - (pad2y-padh) ) + padh  => pad2y;
	}



	//puck

	//sides
	if ( pucky < -boardh || boardh < pucky) { 
		puckvy * -1.0 => puckvy;
		sndtrigger(3);
	}	

	-boardw + padspace + padw => p1f;
	boardw - ( padspace + padw ) => p2f;

	if ( puckx < -boardw || puckx > boardw ) { 

		math.fmod ( puckx + boardw , boardw * 2.0  ) - boardw => puckx;
		if ( puckx < -boardw ) puckx + boardw * 2.0 => puckx;

	}

	else if ( puckx - puckrad < p1f && pucky < pad1y + padh && pucky > pad1y - padh ) { 
		if ( puckx - puckrad + puckvx * -dt  > p1f ) { 
			puckvx * -1.0 => puckvx;
			p1f + ( p1f - ( puckx - puckrad )  ) + puckrad => puckx;
			puckvy + 0.5 * ( pad1vy - puckvy ) => puckvy; //friction transfer...
			sndtrigger(1);
		} 
	}	
	else if ( puckx + puckrad > p2f && pucky < pad2y + padh && pucky > pad2y - padh ) { 
		if ( puckx + puckrad +  puckvx * -dt < p2f ) { 
			puckvx * -1.0 => puckvx; //bouncy
			p2f + ( p2f - ( puckx + puckrad ) ) - puckrad  => puckx;
			puckvy + 0.5 * ( pad2vy - puckvy )  => puckvy; //friction transfer...
			sndtrigger(2);	
		} 
	}		
}


function void actionshred() { 

	while ( true ) { 


		std.sgn( pad1vy ) * math.max( 0.0, std.abs(pad1vy) - 0.2 * dt ) => pad1vy;
		pad1y + pad1vy * dt => pad1y;

		std.sgn( pad2vy ) * math.max( 0.0, std.abs(pad2vy) - 0.2 * dt ) => pad2vy;
		pad2y + pad2vy * dt => pad2y;

		puckx + puckvx * dt => puckx;
		pucky + puckvy * dt => pucky;

		testcollisions();

		1::ms => now;

 	}
}

function void eventshred() { 

	while ( true ) { 
		theeventpoll();
		30::ms => now;
	}

}

function void gluckinitcall() { 

	//MUST be called before any other gluck function

	gluck.Init();

	gluck.InitBasicWindow("ponnng?"); //generate a simple window

	gluck.InitCallbacks(0, 1, 1); //register callbacks for the window
	//arguments monitor ( mouse, motion, keyboard );

	//set up basic camera
	gl.MatrixMode(gl.PROJECTION); 
	gl.LoadIdentity();
	gl.Ortho(-1.0, 1.0, -1.0 , 1.0, -4.0 , 4.0 );

	gl.MatrixMode(gl.MODELVIEW); 
	gl.LoadIdentity();
	gl.ClearColor ( 0.0 , 0.0, 0.3, 0.0 );
}

function void gluckloop( ) { 
	
	gluck.MainLoopEvent(); //...
	if ( gluck.NeedDraw() && running == 1 ) { thedrawloop(); }
	gluck.PostRedisplay();

}

function void gluckshred() {

	gluckinitcall();
	spork ~ eventshred();
	while ( running == 1 ) { 
		gluckloop(); 
		33::ms => now; //30 fps
	}
}

spork ~ gluckshred();
spork ~ audioshred();
spork ~ actionshred();

while ( running == 1 ) { 

	0.3::second => now;

}

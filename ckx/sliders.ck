
"gl.ckx" => (:gl:);
"glu.ckx" => (:glu:);
"gluck.ckx" => (:gluck:);

//convenient gl constants
0 => uint GL_POINTS;
1 => uint GL_LINES;
2 => uint GL_LINE_LOOP;
3 => uint GL_LINE_STRIP;
4 => uint GL_TRIANGLES;
5 => uint GL_TRIANGLE_STRIP;
6 => uint GL_TRIANGLE_FAN;
7 => uint GL_QUADS;
8 => uint GL_QUAD_STRIP;
9 => uint GL_POLYGON;

5889 => uint GL_PROJECTION_MATRIX;
5888 => uint GL_MODELVIEW_MATRIX;

16384 => uint GL_COLOR_BUFFER_BIT;
256   => uint GL_DEPTH_BUFFER_BIT;

0     => uint GLUCK_LEFT_BUTTON;
1     => uint GLUCK_MIDDLE_BUTTON;
2     => uint GLUCK_RIGHT_BUTTON;

0     => uint GLUCK_DOWN;
1     => uint GLUCK_UP;


//our variables

0.0 => float tm;

0.0 => float tb;
0.0 => float tc;
//event info
0   => int curid;
0   => int curtype;

//mouse tracking
0.0 => float mx;
0.0 => float my;
0.0 => float mdx;
0.0 => float mdy;
0.0 => float pmx; 
0.0 => float pmy;
0.0 => float downx; 
0.0 => float downy;

//variables-  and their display qualities
1.0 => float ctla_val;
0.0 => float ctla_x;
-0.5 => float ctla_y;
0   => int ctla_sel;

0.0 => float ctlb_val;
0.5 => float ctlb_x;
-0.5 => float ctlb_y;
0   => int ctlb_sel;

0.0 => float ctlc_val;
-0.5 => float ctlc_x;
-0.5 => float ctlc_y;
0   => int ctlc_sel;

//fizziks

0.0 => float ctlc_dv;
0.0 => float ctlb_dv;

2.0 => float gac;
0.005 => float dt;

0.0 => float cbmag;


//general slider drawing variables

0.06 => float width;
0.02 => float border;
0.9  => float groovelength;
0.03 => float slideheight;
0.08 => float titleheight;

//draw at pos, with val, int selection mode 
function void drawSlider (float val, float x, float y, int selected, string title) { 

	gl.PushMatrix();

	gl.Translatef (x, y, 0.0 );

	//box
	gl.Begin(GL_LINE_LOOP); 
	gl.Color4f(0.0, 0.5, 0.0, 0.75);
	gl.Vertex3f( -(width+border), -border, 0.0 );
	gl.Vertex3f(  (width+border), -border, 0.0 );
	gl.Vertex3f(  (width+border), groovelength+border, 0.0 );
	gl.Vertex3f( -(width+border), groovelength+border, 0.0 );
	gl.End();

	//groove
	gl.Begin(GL_LINES);
	gl.Color4f(0.0,1.0,0.0,0.75 );
	gl.Vertex3f(0.0,0.0,0.0);
	gl.Vertex3f(0.0,groovelength,0.0); 
	gl.End();

	//slider
	gl.PushMatrix();
	gl.Translatef( 0.0, val * groovelength, 0.0);

	gl.Color4f  ( 0.2, 1.0, 0.2, 0.9);
	if ( selected == 1 ) { gl.Color4f(0.5, 1.0, 0.2, 1.0 ); }
	gl.Begin(GL_QUADS);
	gl.Vertex3f (-width, -slideheight, 0.01 );
	gl.Vertex3f ( width, -slideheight, 0.01 );
	gl.Vertex3f ( width, slideheight,  0.01 );
	gl.Vertex3f (-width, slideheight,  0.01 );
	gl.End();

	gl.Color4f  ( 0.0, 0.2, 0.0, 1.0 );
	gl.Begin(GL_LINES);
	gl.Vertex3f ( -width, 0.0, 0.02 );
	gl.Vertex3f (  width, 0.0, 0.02 );
	gl.End();

	gl.PopMatrix();

	gl.PushMatrix();

	gl.Translatef (0.0, groovelength + border, 0.0 );
	gl.Color4f (0.3, 0.6, 0.3, 0.8);
	if ( selected == 2 ) { gl.Color4f(0.5, 1.0, 0.2, 1.0 ); }
	gl.Begin(GL_QUADS);
	gl.Vertex3f ( -(width+border), 0.0,  0.0);
	gl.Vertex3f ( width+border,    0.0,  0.0);
	gl.Vertex3f ( width+border,    titleheight,  0.0);
	gl.Vertex3f ( -(width+border), titleheight,  0.0);	
	gl.End();

	gl.PushMatrix();
	gl.Translatef ( -(width+border), 0.0 , 0.02);
	gl.Scalef (0.0007, 0.0008, 0.0007);
	gl.Color3f ( 0.0, 0.0, 0.0 );
	gluck.StrokeString ( 0, title );
	gl.PopMatrix();
	gl.PopMatrix();

	gl.PopMatrix();

}

//checks whether we've hit the slider ( handle, or menu ) 
function int hitSlider ( float x, float y, float val) { 

	mx - x => float locx;
	my - y => float locy;

	//hit slider ?
	if ( locy - ( val * groovelength ) < slideheight && locy - ( val * groovelength ) > -slideheight && locx < width && locx > -width ) { return 1; } 
	//hit title ?
	if ( locy < groovelength + border + titleheight && locy > groovelength + border  && locx < (width+border) && locx > -(width+border) ) { return 2; }  

	return 0;
}

function int hitGroove ( float x, float y ) { 
	mx - x => float locx;
	my - y => float locy;
	if ( -width < locx && locx < width && -slideheight < locy && locy < groovelength + slideheight ) { return 1; }	
	return 0;
}

//value adjustment for slider ( click vs motion ) 
function float positionAdjustSlider ( float x, float y ) { 
	return ( ( my - y ) / groovelength );
}

function float motionAdjustSlider ( float x, float y, float val ) { 
	 ( val + ( mdy / groovelength ) ) => float nv;
	 return math.min ( math.max ( 0.0, nv ) , 1.0 );
}

function void mposEvent ( int mid ) { 
	//map to ndc, store last
	mx => pmx;
	my => pmy;
	 2.0 * ( (float) gluck.GetEventX(mid) / (float) gluck.GetViewDims(0) ) - 1.0  => mx;
	-2.0 * ( (float) gluck.GetEventY(mid) / (float) gluck.GetViewDims(1) ) + 1.0  => my;
	mx - pmx => mdx;
	my - pmy => mdy; 	
}

function void slidermouse ( int button , int state ) { 

	//mouse clicks

	if ( state == GLUCK_DOWN ) { 

		mx => downx;
		my => downy;

		hitSlider (ctla_x, ctla_y, ctla_val) => ctla_sel;
		hitSlider (ctlb_x, ctlb_y, ctlb_val) => ctlb_sel;
		hitSlider (ctlc_x, ctlc_y, ctlc_val) => ctlc_sel;

	}
	if ( state == GLUCK_UP ) { 
		//for value-selected boxes...test t		

		if ( mx == downx && my == downy ) { //click and release in same location
			if ( hitGroove(ctla_x, ctla_y) == 1 ) { positionAdjustSlider ( ctla_x, ctla_y) => ctla_val;}
			if ( hitGroove(ctlb_x, ctlb_y) == 1 ) { positionAdjustSlider ( ctlb_x, ctlb_y) => ctlb_val;  0.0 => ctlb_dv; }
			if ( hitGroove(ctlc_x, ctlc_y) == 1 ) { positionAdjustSlider ( ctlc_x, ctlc_y) => ctlc_val;  0.0 => ctlc_dv; }
		}

		//clear all ( unless toggle is down? )
		0 => ctla_sel;
		0 => ctlb_sel;
		0 => ctlc_sel;
	}
}

function void slidermotion ( int button , int state ) { 
	//mouse drag

	if ( ctla_sel == 1 )  { motionAdjustSlider ( ctla_x, ctla_y, ctla_val) => ctla_val; }
	if ( ctla_sel == 2 )  { ctla_x + mdx => ctla_x; ctla_y + mdy => ctla_y; }  

	if ( ctlb_sel == 1 )  { motionAdjustSlider ( ctlb_x, ctlb_y, ctlb_val) => ctlb_val; }
	if ( ctlb_sel == 2 )  { ctlb_x + mdx => ctlb_x; ctlb_y + mdy => ctlb_y; }  

	if ( ctlc_sel == 1 )  { motionAdjustSlider ( ctlc_x, ctlc_y, ctlc_val) => ctlc_val; }
	if ( ctlc_sel == 2 )  { ctlc_x + mdx => ctlc_x; ctlc_y + mdy => ctlc_y; }  

}


function void theeventloop() { 

	while ( gluck.HasEvents() ) { 

		gluck.GetNextEvent() => curid;
		gluck.GetEventType(curid) => curtype;

		if ( curtype == 0 ) {  // mouse click
			mposEvent(curid);
			slidermouse(gluck.GetEventButton(curid), gluck.GetEventState(curid));
		}
		else if ( curtype == 1 ) { //depressed motion
			mposEvent(curid);
			slidermotion(gluck.GetEventButton(curid), gluck.GetEventState(curid));
		}
	 	else if ( curtype == 3 ) { //keyboard
			gluck.GetEventKey(curid) => uint k;
		}
	}

}

function void thedrawloop() { 

		gl.ClearColor( ctlc_val, ctlb_val, ctla_val, 0.0);

		gl.Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		gl.PushMatrix();
 		gl.Translatef ( 0.0, 0.0, -0.5 ); 

		gl.Color4f(0.8, 0.0, 0.0, 0.5 );
		gl.Begin( GL_QUADS ) ;
		gl.Vertex3f ( -1.0, -1.0 + 2.0 * math.sqrt(ctlc_val) , 0.0 );
		gl.Vertex3f (  1.0, -1.0 + 2.0 * math.sqrt(ctlc_val) , 0.0 );
		gl.Vertex3f (  1.0, -1.0 , 0.0 );
		gl.Vertex3f ( -1.0, -1.0 , 0.0 );



		gl.End();

		gl.Translatef ( 0.0, 0.0 , 0.1 );
		gl.Color4f(0.0, 0.8, 0.0, 0.5 );
		gl.Begin( GL_QUADS ) ;

		gl.Vertex3f ( -1.0, -1.0 + 2.0 * math.sqrt(ctlb_val) , 0.0 );
		gl.Vertex3f (  1.0, -1.0 + 2.0 * math.sqrt(ctlb_val) , 0.0 );
		gl.Vertex3f (  1.0, -1.0 , 0.0 );
		gl.Vertex3f ( -1.0, -1.0 , 0.0 );

//		gl.Vertex3f ( -1.0,  1.0 , 0.0 );
//		gl.Vertex3f (  1.0,  1.0 , 0.0 );
//		gl.Vertex3f (  1.0,  1.0 - 2.0 * math.sqrt(ctlb_val) , 0.0 );
//		gl.Vertex3f ( -1.0,  1.0 - 2.0 * math.sqrt(ctlb_val) , 0.0 );
		gl.End();

		gl.PopMatrix();

		gl.PushMatrix();


		gl.Translatef ( -1.0 ,-1.0 , 0.0);
		gl.Scalef ( 2.0, 2.0 , 1.0  );

		gl.Color4f ( 1.0, 1.0 , 1.0, 0.5);

		gl.Begin(GL_LINES);

		gl.Vertex3f ( tm - math.floor(tm), 0.0 ,  -0.2 );
		gl.Vertex3f ( tm - math.floor(tm), 1.0 , -0.2 );
		gl.End();

		gl.Color4f ( 0.0, 0.0 , 0.0, 0.5);
		gl.Begin(GL_LINES);

		gl.Vertex3f ( tb - math.floor(tb), 0.0 ,  -0.2 );
		gl.Vertex3f ( tb - math.floor(tb), 1.0 , -0.2 );
		gl.End();

		gl.Begin(GL_LINES);

		gl.Vertex3f ( tc - math.floor(tc), 0.0 ,  -0.2 );
		gl.Vertex3f ( tc - math.floor(tc), 1.0 , -0.2 );
		gl.End();

		gl.Begin (GL_QUADS );
		gl.Color4f ( 0.4, 0.0, 0.0, 0.75 - (tm - tb) );
		gl.Vertex3f ( tb - math.floor(tb) , 0.0, -0.1);
		gl.Vertex3f ( tb - math.floor(tb) , 1.0, -0.1);
		if ( tm - math.floor(tm) > tb - math.floor(tb)) { 
			gl.Vertex3f ( tm - math.floor(tm)  , 1.0, -0.1);
			gl.Vertex3f ( tm - math.floor(tm)  , 0.0, -0.1);
		}
		else { 
			gl.Vertex3f ( 1.0  , 1.0, -0.1);
			gl.Vertex3f ( 1.0  , 0.0, -0.1);

			gl.Vertex3f ( 0.0  , 0.0, -0.1);
			gl.Vertex3f ( 0.0  , 1.0, -0.1);
			gl.Vertex3f ( tm - math.floor(tm)  , 1.0, -0.1);
			gl.Vertex3f ( tm - math.floor(tm)  , 0.0, -0.1);

		}

		gl.Color4f ( 0.0, 0.4, 0.0, 0.75 - (tm - tc) );
		gl.Vertex3f ( tc - math.floor(tc)  , 0.0, -0.08);
		gl.Vertex3f ( tc - math.floor(tc)  , 1.0, -0.08);
		if ( tm - math.floor(tm) > tc - math.floor(tc)) { 
			gl.Vertex3f ( tm - math.floor(tm)  , 1.0, -0.08);
			gl.Vertex3f ( tm - math.floor(tm)  , 0.0, -0.08);
		} 
		else { 
			gl.Vertex3f ( 1.0  , 1.0, -0.1);
			gl.Vertex3f ( 1.0  , 0.0, -0.1);

			gl.Vertex3f ( 0.0  , 0.0, -0.1);
			gl.Vertex3f ( 0.0  , 1.0, -0.1);
			gl.Vertex3f ( tm - math.floor(tm)  , 1.0, -0.1);
			gl.Vertex3f ( tm - math.floor(tm)  , 0.0, -0.1);

		}

		gl.End(); 

		gl.PopMatrix();

		gl.LineWidth(2.0);

		drawSlider ( ctla_val, ctla_x, ctla_y, ctla_sel, "-x-" );
		drawSlider ( ctlb_val, ctlb_x, ctlb_y, ctlb_sel, "pip" );
		drawSlider ( ctlc_val, ctlc_x, ctlc_y, ctlc_sel, "pop" );

		gl.Flush();
		gluck.SwapBuffers();

}

function void ricochet( float loud, float mfreq) { 
	noise nose => biquad bork => dac;
	0.99 => bork.prad;
	1    => bork.eqzs;
	mfreq => bork.pfreq;
	loud => float borkg;
	borkg => bork.gain;
	while ( borkg > 0.0 ) { 
		borkg => bork.gain;
		borkg - ( 0.006 * loud ) => borkg;
		1::ms => now;
	}
}

function void audioshred( ) { 

	while ( true ) { 
		//do something!
		5::ms => now;
	}
 
}

function void controlshred() { 
	//advancing time, messing with control values

	while ( true ) { 

		//bouncybouncy-c
		if ( ctlc_sel != 1 ) { 
			if ( ctlc_val > 0.0 || ctlc_dv > 0.0 ) { 
			        ctlc_dv  - (gac * dt) => ctlc_dv;
				ctlc_val + (ctlc_dv * dt) => ctlc_val;
			}
			else if ( ctlc_val < 0.0 ) { 
				if ( ctlc_dv < -0.01 ) { 
					if ( ctlb_val == 0.0 ) { 			
						 ctlb_dv - ctlc_dv * ( 1.0 - ctla_val )  => ctlb_dv; 
					}
					ctlc_dv * -ctla_val => ctlc_dv; 
					//0.1 + ctlc_dv * 0.3 => cbmag;
					//cbmag=> stdout;
					tm => tc;
					spork ~ ricochet( 0.2 , 880.0 );
				}
				else { 0.0 => ctlc_dv; }
				0.0 => ctlc_val;
			}
		}
		else { 	0.0 => ctlc_dv; }


		//bouncybouncy-b
		if ( ctlb_sel != 1 ) { 
			if ( ctlb_val > 0.0 || ctlb_dv > 0.0 ) { 
			        ctlb_dv - (gac * dt) => ctlb_dv;
				ctlb_val + (ctlb_dv * dt) => ctlb_val;
//				ctlb_val => stdout;
			}
			else if ( ctlb_val < 0.0 ) { 
				if ( ctlb_dv < -0.01 ) {
					if ( ctlc_val == 0.0 ) {  			
						ctlc_dv - ctlb_dv *  ( 1.0 - ctla_val)  => ctlc_dv;
					}
					ctlb_dv * -ctla_val => ctlb_dv; 
					tm => tb;
					spork ~ ricochet( 0.2, 1100.0 );
				}
				else { 0.0 => ctlb_dv; }
				0.0 => ctlb_val;
			}
		}
		else { 	0.0 => ctlb_dv; }

		tm + dt => tm;
		5::ms => now;
	}
}

function void gluckinitcall() { 

	//MUST be called before any other gluck function

	gluck.Init();

	gluck.InitBasicWindow("sllliderrsss"); //generate a simple window

	gluck.InitCallbacks(1, 1, 1); //register callbacks for the window
	//arguments monitor ( mouse, motion, keyboard );

	//set up basic camera
	gl.MatrixMode(GL_PROJECTION_MATRIX); 
	gl.LoadIdentity();
	gl.Ortho(-1.0, 1.0, -1.0 , 1.0, -4.0 , 4.0 );

	gl.MatrixMode(GL_MODELVIEW_MATRIX); 
	gl.LoadIdentity();
	gl.ClearColor ( 0.0 , 0.0, 0.3, 0.0 );
}

function void gluckloop( ) { 
	
	gluck.MainLoopEvent(); //...

	if ( gluck.NeedDraw()  ) { thedrawloop(); }
	if ( gluck.NeedEvent() ) { theeventloop(); }

	gluck.PostRedisplay();

}

function void gluckshred() {

	gluckinitcall();

	while ( true ) { 
		gluckloop(); 
		33::ms => now; //30 fps
	}
}

spork ~ gluckshred();
spork ~ audioshred();
spork ~ controlshred();

while ( true ) { 
	//sit on yer thumbs;
	1::second => now;
}

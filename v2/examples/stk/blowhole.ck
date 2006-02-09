// patch
BlowHole hole => dac;

// scale
[0, 2, 4, 7, 9, 11] @=> int scale[];

// infinite time loop
while( true )
{
  // change parameters
  if( std.rand2f(0,1) > .75 )
  {
    std.rand2f( 32, 128 ) => float stiffness;
    std.rand2f( 32, 128 ) => float noisegain;
    std.rand2f( 0, 128 ) => float tonehole;
    std.rand2f( 0, 128 ) => float register;
    std.rand2f( 32, 128 ) => float pressure;

    <<< "going...", "" >>>;
    <<< "reed stiffness:", stiffness >>>;
    <<< "noise gain:", noisegain >>>;
    <<< "tonehole state:", tonehole >>>;
    <<< "register state:", register >>>;
    <<< "breath pressue:", pressure >>>;

    // reed stiffness
    hole.controlChange( 2, stiffness );
    // noise gain
    hole.controlChange( 4, noisegain );
    // tonehole state
    hole.controlChange( 11, tonehole );
    // register state
    hole.controlChange( 1, register );
    // breath pressue
    hole.controlChange( 128, pressure );
  }

  // set freq
  scale[std.rand2(0,scale.cap()-1)] => int note;
  33 + std.rand2(0,4)*12 + note => std.mtof => hole.freq;
  <<< "note: ", std.ftom( hole.freq() ) >>>;

  // go
  .8 => hole.noteOn;

  // advance time
  1::second => now;
}

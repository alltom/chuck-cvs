// patch
BlowHole hole => dac;

// scale
[0, 2, 4, 7, 9, 11] @=> int scale[];

<<< "reed stiffness:", hole.reed() >>>;
<<< "noise gain:", hole.noiseGain() >>>;
<<< "tonehole state:", hole.tonehole() >>>;
<<< "register state:", hole.vent() >>>;
<<< "breath pressure:", hole.pressure() >>>;

// infinite time loop
while( true )
{
  // change parameters
  if( std.rand2f(0,1) > .75 )
  {
    std.rand2f( 0, 1 ) => hole.reed;
    std.rand2f( 0, 1 ) => hole.noiseGain;
    std.rand2f( 0, 1 ) => hole.tonehole;
    std.rand2f( 0, 1 ) => hole.vent;
    std.rand2f( 0, 1 ) => hole.pressure;

    // print
    <<< "going...", "" >>>;
    <<< "reed stiffness:", hole.reed() >>>;
    <<< "noise gain:", hole.noiseGain() >>>;
    <<< "tonehole state:", hole.tonehole() >>>;
    <<< "register state:", hole.vent() >>>;
    <<< "breath pressure:", hole.pressure() >>>;
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

// oscillator demo
// - philipd

//control oscillators - send to blackhole
phasor x => blackhole; 
sinosc modw => blackhole;

60.0 => x.freq;
0.125 => modw.freq;

x => sinosc s    => Envelope se => dac;
x => triosc t    => Envelope te => dac; 

t => sinosc ts   => Envelope tse => dac;
ts => triosc tst => Envelope tste => dac;
x => pulseosc p  => Envelope pe => dac;
x => sawosc w    => Envelope we => dac;
x => sqrosc q    => Envelope qe => dac;

2 => s.sync;     // 2 = sync to input
2 => t.sync;
2 => ts.sync;
2 => tst.sync;
2 => p.sync;
0.25 => p.width;
2 => w.sync;
2 => q.sync;

0.1 => se.time => te.time => tse.time => tste.time => 
       pe.time => we.time => qe.time;

0 => int c;

fun void vary() { 
    while ( true ) { 
        modw.last => t.width;
        modw.last => p.width;
        modw.last => w.width;
        1::ms => now;
    }
}

spork ~vary();

while ( true ) {
    if ( c % 7 == 0 ) { 1.0 => se.target; 0.0 => qe.target;}
    if ( c % 7 == 1 ) { 1.0 => te.target; 0.0 => se.target;}
    if ( c % 7 == 2 ) { 1.0 => tse.target; 0.0 => te.target;} 
    if ( c % 7 == 3 ) { 1.0 => tste.target; 0.0 => tse.target;} 
    if ( c % 7 == 4 ) { 1.0 => pe.target; 0.0 => tste.target;}
    if ( c % 7 == 5 ) { 1.0 => we.target; 0.0 => pe.target;}
    if ( c % 7 == 6 ) { 1.0 => qe.target; 0.0 => we.target;}
    c++;
    1::second => now;	
}

//delayph.ck  sound check for delayph class

Mandolin m => dac;
Wurley wur => delayph dp => dac;
Wurley w2 => dac;
220.0 => w2.freq;
440.0 => wur.freq;

now => stdout;
6::second => dp.max;
0.05::second => dp.window;
1::second => dp.delay;
5::second => now;
now => stdout;
2::second => now;
"end" => stdout;
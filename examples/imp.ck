// impulse generator is cool...
// this demo is not

// connect impulse generator
impulse i => dac;

// emit impulse every so often
1000 => int a;
1.0 => float v;
while( 1 )
{
    a::samp => now;
    v => i.value;
    a - 2 => a;
    if( a <= 0 ) 1000 => a;
}

// tick tock

// wait until the start of the next second
second - (now % second) => now;

// infinite time loop
while( true )
{
    // print now in seconds
    <<< "now:", (now / second) $ int, "second(s)" >>>; 
    1::second => now;
}

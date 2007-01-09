// THX emulator, Perry R. Cook, Jan. 8, 2007
//
// F-1,  B1b,   F1,  B2b,   F2,  B3b,   F3,  A5,   F4, A6
[ 29.0, 87.5,116.0,175.0,233.0,350.0,524.0,880.0,1048,1760,
 29.0, 87.5,116.0,175.0,233.0,350.0,524.0,880.0,1048,1760,
 29.0, 87.5,116.0,175.0,233.0,350.0,524.0,880.0,1048,1760 ] @=> float targets[];
float initials[30];
float deltas[30];
sawosc s[30];
gain gl[30];
gain gr[30];
JCRev rl => dac.left;
JCRev rr => dac.right;

0 => int i => int j;

for (0 => i; i<30;  i+1 => i)   {
    std.rand2f(200.0,800.0) => initials[i] => s[i].freq; // random freqs.
    (targets[i] - initials[i]) / 10000.0 => deltas[i];  // 10 sample updates
    0.1 => s[i].gain;
    std.rand2f(0.0,1.0) => gl[i].gain;                  // random
    1.0 - gl[i].gain() => gr[i].gain;                   // panning
    s[i] => gl[i] => rl;                                // hook up
    s[i] => gr[i] => rr;                                // all the oscs
}

10000 :: samp => now;                                   // steady cluster

while(j < 10000)        {
    for (0 => i; i<30;  i+1 => i)       {
        initials[i] + (deltas[i]*j) => s[i].freq;       // sweep freqs.
    }
    j + 1 => j;
    10 :: samp => now;
}
while (j < 30000)       {                               // hold chord
    10 :: samp => now;
    j + 1 => j;
}
while (j < 35000)       {
    for (0 => i; i<30;  i+1 => i)       {
        0.1 * (35000 - j) / 10000.0 => s[i].gain;       // decay gains
    }
    10 :: samp => now;
    j + 1 => j;
}
60000 :: samp => now;                                   // reverb tail

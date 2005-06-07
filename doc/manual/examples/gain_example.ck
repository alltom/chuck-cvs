        noise n => gain g => dac;  
        sinosc s => g;  
        .3 => g.gain;  
        while( true ) { 100::ms => now; } 

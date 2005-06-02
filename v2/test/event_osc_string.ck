OSC_Recv recv;

6448 => recv.port;
recv.listen();

<<<"string receive test started">>>;
function void trigger_s () { 
	<<<"function started">>>;
	OSC_Addr oscev;
	oscev => recv.add_address;
	"/foo/zoop,s" => oscev.set;
	while ( true  ) { 
		oscev => now;	
		<<<"event occurred">>>;
		while ( oscev.nextMesg() != 0 ) { 
			<<<"fetching string">>>;
			string c;
			oscev.getString() => c;	
			<<<"zoop">>>;	
			<<<c>>>;
		}
	}
}

spork ~ trigger_s();


while ( true ) { 
	1::second => now;
}


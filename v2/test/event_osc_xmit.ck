
OSC_Send xmit;
xmit.setHost("localhost", 6448 );

while ( true ) { 
	xmit.startMesg( "/foo/zoop", ",s" );
	xmit.addString( "rats" );
	<<<"transmitted">>>;
	1::second => now;
}
<<<"goodbye">>>;
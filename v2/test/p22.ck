// for loop ( factorial ) , -=>, *=>

fun int factorial ( int arg ) { 
	1 => int r;
	arg => int b;
	for ( b ; b > 1 ; 1 -=> b ) { 
		b *=> r;
	}
	return r;
}

factorial ( 8 ) => int x;
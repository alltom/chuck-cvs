// linked list Fifo;

class ListItem { 
	int data;
	ListItem @ next;
	fun void push ( ListItem it ) { 
		if ( next == null ) it @=> next;
		else next.push(it);
	} 
}

class Fifo { 
	ListItem @ root;
	fun void push( ListItem it) { 
		if ( root == null ) it @=> root;
		else root.push(it);
	}
	fun void empty() {  return ( root == null ); } 
	fun ListItem pop() { 
		if ( empty() ) return null;
		root @=> ListItem @ ret;
		root.next @=> root;
		return ret;
	}
}

Fifo f;

f.push( ListItem n );
f.push( ListItem p );
f.pop() => ListItem @ l;

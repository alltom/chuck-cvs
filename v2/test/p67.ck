// linked list stack;

class ListItem { 
	int data;
	ListItem @ next;
	fun void push ( ListItem it ) { 
		if ( next != null ) next.push(it);
		else it => next;
	} 
	fun ListItem pop () { 
		if ( next.isLast() ) {
			next => ListItem @ n;
			null => next; 
			return n;
		}
		else return next.pop();
	}
	fun boolean isLast() { 
		return ( next == null );
	}
}

class Stack { 
	int num;
	ListItem @ root = null;
	fun void push( ListItem it) { 
		if ( root == null ) it => root;
		else root.push(it);
	}
	ListItem pop() { 
		if ( root == null ) return null;
		else if ( root.isLast() ) { 
			root => ListItem @ ret ;
			null => root;
			return ret;
		}
		else { 
			return root.pop();
		}
	}
}

Lifo f;

f.push( ListItem n );
f.push( ListItem p );

// class with function taking class as argument

class Other { 
	int x;
	int y;
}

class Obj { 
	int x;
	int y;
	public void set( Other a ) { 
		a.x => x;
		a.y => y;
	}
} 

Other a;
Obj o;
o.set(a);


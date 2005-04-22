//virtual table

class Top { 
	function int value() { 
		return 1;
	}
}

class Bottom extends Top { 
	function int value() {
		return 2;
	}
}

Top t;
Bottom b;
b @=> Top @ b2t;

<<<"Bottom Class">>>;
<<<b.value()>>>;

<<<"Top Class">>>;
<<<t.value()>>>;

<<<"Bottom Cast to Top">>>;
<<<b2t.value()>>>;

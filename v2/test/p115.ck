//virtual table

class Top { 
	function int topValue() { 
		return 1;
	}
}

class Bottom extends Top { 
	function int bottomValue() {
		return 2;
	}
}

Top t;
Bottom b;
b @=> Top @ b2t;


<<<"Bottom Class">>>;
<<<b.topValue()>>>;
<<<b.bottomValue()>>>;

<<<"Top Class">>>;
<<<t.topValue()>>>;
<<<t.bottomValue()>>>; //error!

<<<"Bottom Cast to Top">>>;
<<<b2t.topValue()>>>;
<<<b2t.bottomValue()>>>; //error!
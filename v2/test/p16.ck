// cast a parent class array to a subclass array
// this should fail.

class Parent
{
    public void foo() { }
    int index;
}

class Child extends Parent { 
    int value;
}

Parent p[4];
Child c[4];
p @=> c;
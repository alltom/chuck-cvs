// cast parent class to subclass

class Parent
{
    public void foo() { }
    int index;
}

class Child extends Parent { 
    int value;
}

Parent p;
Child c;
p @=> c;
// class with a currently undeclared, undefined class as member

class ListItem
{
    public void foo() { }
    ListValue myval;
    int index;
}

class ListValue { 
    int value;
}

ListItem root;

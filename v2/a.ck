
// adc => sndbuf => dac;

1 => int i;

1 + 1 => i;

class Foo
{
    int f;
    int a1;

    public int foo()
    {
        f + a1 => f;
        return 1;
    }

    int g;

    public int bar()
    {
        return 0;
    }
}


class Bar extends Fooa
{
    int c;
}

Foo foo;
Bar bar;
foo.f + bar.c => foo.a1;

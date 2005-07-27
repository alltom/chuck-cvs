// multi-dimensional array instantiation
int a[2][2][2];
float b[2][2][2];
5 => a[1][1][1] => b[1][1][1];

// multi-dimensional array initialization
[ [1,2], [3,4] ] @=> int c[][];

if( a[1][1][1] == 5 && b[1][1][1] == 5.0 && c[0][1] == 2 ) <<<"success">>>;

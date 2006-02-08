// big number
1000000000000000000000000000.0 => float n;
// overflow it
repeat( 20 ) n *=> n;
// test it
<<< math.isinf( n ) >>>;
// bye sanity
0 *=> n;
// test it
<<< math.isnan( n ) >>>;

// more
<<< 1.0 / 0.0 => math.isinf >>>;
<<< 0.0 / 0.0 => math.isnan >>>;

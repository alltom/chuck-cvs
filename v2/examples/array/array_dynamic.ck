// instantiate int array
float argh[0];

// print out relevant info
<<< "array size:", argh.size(), "array capacity:", argh.capacity() >>>;

// append items (array should grow dynamically as needed)
argh << 3.0 << 4 << 5;

// print out relevant info
<<< "array size:", argh.size(), "array capacity:", argh.capacity() >>>;

// print out elements
<<< "contents:", argh[0], argh[1], argh[2] >>>;


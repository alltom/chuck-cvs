//a mini-compatability library for 
//math functions that might be missing 
//on different systems.

#ifndef _UTIL_MATH_H_INCLUDED_
#define _UTIL_MATH_H_INCLUDED_

#include <math.h>
#ifdef WIN32
	double round(double a);
	double trunc(double a);
	double remainder(int a, int b);
#endif

#endif 

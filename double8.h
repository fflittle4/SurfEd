#pragma once
/*
bring the functionality of bezel.h/cpp to c++, anticipate the support of jacks
*/
#include "double3.h"
class double8
{
public:
	double8();				// DEFAULT: make this the Identity for rotate and scale followed by 0 displacement
//	double8(double *R);
	double8(double(R)[3]);
//	double8(double(R)[8]);
	double8(float *R);
	~double8();

	void operator !();			// make bezel view this  in other words loadA_ to MODELVIEW
	void operator +(double8 *);	// compose from left to right transform (data is rows)
	void operator -(double8 *);	// compose with inverse
	void operator ^(double8 *);	// inverse compose

	double8 operator +(double8 s);
	void glMult(void);			// make bezel view this  in other words multA to MODELVIEW
	void from(double *A, double *R);
	void to(double *d, double *s);

	void from(double3, double3);
	void from(double8, double8);
	void to(double3 a, double3 b);
	void gljack(double jacksize);
	void glblack(double jacksize);
	void gljck(double jacksize);



//private:
	double r[8];	// 3 displacement 4 rotation 1 scale
};


#pragma once
#include "rsq.h"
#include <math.h>
/*	really basic vector arithmetic:
* V*b
* cross product
^ dot product
-=
+
-
UNIARY

	+	NORM SQUARED
	-	uniary -
	~   norm
	*   1/norm


*/

// uniary + op is norm squared
// binary ^ is  dot product
// * post multiply by double
// -= like double...
// uniary - ...
extern double  sqrt(double _X);
class double3
{
public:
    // constructors
    double3() {}
    double3(double (V)[3])
    {
        for (int i = 3; i--;)
            v[i] = V[i];
    };
    double3(double(*V)[3])
    {
        for (int i = 3; i--;)
            v[i] = *V[i];
    };

    double3(double x, double y, double z)
    {
        v[0] = x;
        v[1] = y;
        v[2] = z;
    };


    double3(double *V, double *T) 	// part of V perpendicular to T
    {
        double b = 1.e-80, a = 0;
        for (int i = 3; i--;)
        {
            a += V[i] * T[i];
            b += T[i] * T[i];
        };
        a /= b;
        for (int i = 3; i--;)
        {
            v[i] = V[i] - a*T[i];
        }
    }

    double3(double3 A, double3 B)
    {

        int i;
        double b = 1.e-80, a = 0;
        for (i = 3; i--;)
        {
            a += A.v[i] *B.v[i];
            b += B.v[i] * B.v[i];
        };
        a /= b;

        for ( i = 3; i--;)
        {
            v[i] = A.v[i] - a * B.v[i];
        }
    }
    ~double3() {};

    double operator +() 	// uniary + returns norm squared
    {
        return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    }
    double operator ^ (double3  b)  // binary ^ is dot product
    {
        return(v[0] * b.v[0] + v[1] * b.v[1] + v[2] * b.v[2]);
    }
    double3 operator * (double b)
    {
        double3 r;
        for (int i = 3; i--;)
            r.v[i] = v[i] * b;
        return r;
    }
    double3 operator *= (double a)
    {
        for (int i = 3; i--;)
            v[i] *= a;
        return *this;
    }
//	virtual double3(double4 a);
    double3 operator / (double b)
    {
        double3 r;
        for (int i = 3; i--;)
            r.v[i] = v[i] / b;
        return r;
    }
    double3 operator -= (double3  b)
    {
        double3 r;
        for (int i = 3; i--;)
            r.v[i] = (v[i] -= b.v[i]);
        return r;
    }
    // binary
    double3 operator + (double3  b)
    {
        double3 r;
        for (int i = 3; i--;)
            r.v[i] = v[i] + b.v[i];
        return r;
    }
    double3 operator - (double3  b)
    {
        double3 r;
        for (int i = 3; i--;)
            r.v[i] = v[i] - b.v[i];
        return r;
    }
    double3 operator * (double3  b) 	//Cross Product
    {
        double3 r;
        int j = 0;
        for (int i = 3; i--; j = i)
            r.v[i] = v[j] * b.v[3 - i - j];
        return r;
    }
    //uniary
    double3  operator - ()
    {
        double3 r;
        for (int i = 3; i--;)
            r.v[i] = -v[i];
        return r;
    }

    double operator ~()
    {
        return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    }
    double operator &()
    {
        return rsq(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    }
    double3 operator >> (double *a)  // get
    {
        for (int i = 3; i--;)
            a[i] = v[i];
        return *this;
    }
    void get(double *n)
    {
        for (int i = 3; i--;)
            n[i] = v[i];
    }
    void lerp(double a, double *B, double *R)
    {
        double ac = 1 - a;
        for (int i = 3; i--;)
            R[i] = v[i] * ac + B[i] * a;
    }
    void gl(void);

    double v[3];
};

class double4
{
public:
    // constructors
    double4() {}
//		double4(const double4 a){			for (int i = 4; i--;)q[i] = a.q[i];		}
    double4(double *Q)
    {
        for (int i = 4; i--;)
            q[i] = Q[i];
    }
    double4(double w,double3 a)
    {
        q[0] = w;
        for (int i = 3; i--;)
            q[i+1] = a.v[i];
    }
    double4(double w, double x, double y, double z)
    {
        q[0] = w;
        q[1] = x;
        q[2] = y;
        q[3] = z;
    }
    double4  operator - () 	// negative
    {
        double4 r;
        for (int i = 4; i--;)
            r.q[i] = -q[i];
        return r;
    }
    double4  operator ~  ()  // conjugate
    {
        double4 r;
        for (int i = 1; i<4; i++)
            r.q[i] = -q[i];
        return r;
    }
//		double4 operator >> (double3 *a) {			for (int i = 3; i--;)a->v[i] = q[i + 1];			return;		}
//		double4 operator >> (double3 a) {
//			for (int i = 3; i--;)a.v[i] = q[i + 1];
//			return this;
//		}

    double operator & ()
    {
        return rsq(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
    }
    double operator * ()
    {
        return (q[0] * q[0] + q[1] *q[1] + q[2] * q[2] + q[3] * q[3]);
    }
//		double operator ~(){
//			return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
//		}
    /* quaternion multiply */
    double4  operator *  (double4 B)
    {
        double4 r;

        r.q[0] = q[0] * B.q[0] - q[1] * B.q[1] - q[2] * B.q[2] - q[3] * B.q[3];
        r.q[1] = q[0] * B.q[1] + q[1] * B.q[0] + q[2] * B.q[3] - q[3] * B.q[2];
        r.q[2] = q[0] * B.q[2] + q[2] * B.q[0] + q[3] * B.q[1] - q[1] * B.q[3];
        r.q[3] = q[0] * B.q[3] + q[3] * B.q[0] + q[1] * B.q[2] - q[2] * B.q[1];
        return r;
    }

    double q[4];
};
#if 0
double3::double3(double4 a)
{
    for (int i = 3; i--;)
        v[i] = a.q[i + 1];
};

#endif
//extern double3 Rotate(double3 a, double4 R);
//extern "C" void plerp(double a, double *A, double*B, double *R);

//extern "C" void lerp(double a, double *A, double*B, double *R);

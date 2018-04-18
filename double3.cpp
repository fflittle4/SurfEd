#include "double3.h"
double3 Axis(double4 c)
{
    // and because I can't write a constructor for double3(double4)...
    double3 r;
    for (int i = 3; i--;)
        r.v[i] = c.q[i + 1];
    return r;

}
//double3 Rotate(double3 a, double4 R)
//{
 //   return Axis(R *  double4(0., a) * ~R);
//}


//#include "stdafx.h"
//#pragma warning( disable : 4244)
void to_object(float q[8], double *ar, float d[8]);
void from_objectd(float q[8], double *ar, double x[3]);
void qf_rot(float *t, float *q, float *r);
void doXkey(void);
void doYkey(void);
void doZkey(void);

struct site
{
    struct site *next;
    char *name;
    float q[8];
    long flag;
} bezel;
double fix[8] = { 0., 0., 0., 1.,0.,0.,0.,1., };


void to_object(float object[8], double *ar, float r[3])
{
    int i;
    float d[8];

    for (i = 3; i--;)
        d[i] = ar[i] - object[i];
    // the inverse of the rotation&scale
    d[3] = object[3];
    for (i = 4; i < 7; i++)
        d[i] = -object[i];
    d[7] = 1. / object[7];
    qf_rot(d, d + 3, r);
}

void from_objectd(float object[8], double *ar, double x[3])
{
    int i;
    float d[3];

    for (i = 3; i--;)
        d[i] = ar[i];
    qf_rot(d, object + 3, d);
    for (i = 3; i--;)
        x[i] = d[i] + object[i];
}

#define aM_SQRT1_2  0.707106781186547524401

//extern double fix[8];


void doZkey(void)
{
    float d[8];
    double dd[3], DE[3];
    static int eot = 0;
    to_object(bezel.q, fix, d);
    for (int i = 3; i--;)
        dd[i] = d[i];
    bezel.q[3] = eot ? 0.f : 1.f;
    bezel.q[4] = !eot ? 0.f : 1.f;
    bezel.q[5] = bezel.q[6] = 0.f;
    from_objectd(bezel.q, dd, DE);
    for (int i = 3; i--;)
        bezel.q[i] += fix[i] - DE[i];

    eot = !eot;
    return;

}

void doXkey(void)  // 'X'
{
    float d[8];
    double dd[3], DE[3];
    static int eot = 0;
    to_object(bezel.q, fix, d);
    for (int i = 3; i--;)
        dd[i] = (double)d[i];
    bezel.q[3] = (float)aM_SQRT1_2;
    bezel.q[5] = eot ? (float)aM_SQRT1_2 : -(float)aM_SQRT1_2;
    bezel.q[4] = bezel.q[6] = 0.;
    from_objectd(bezel.q, dd, DE);
    for (int i = 3; i--;)
        bezel.q[i] += fix[i] - DE[i];

    eot = !eot;
    return;

}
void doYkey(void) // 'Y'
{
    float d[8];
    double dd[3], DE[3];
    static int eot = 0;
    to_object(bezel.q, fix, d);
    for (int i = 3; i--;)
        dd[i] = (double)d[i];

    bezel.q[3] = eot ? (float)aM_SQRT1_2 : -(float)aM_SQRT1_2;;
    bezel.q[4] = -(float)aM_SQRT1_2;
    bezel.q[5] = bezel.q[6] = 0.;
    from_objectd(bezel.q, dd, DE);
    for (int i = 3; i--;)
        bezel.q[i] += fix[i] - DE[i];

    eot = !eot;
    return;

}

//#include <gl/GL.h>
#if !0
#include "double8.h"
void loadA_(float *Q)
{
    double8 t(Q);
    !t;
}
#else
void loadA_(float *Q)
{
    float m[4][4];
    float a, b, c, d;
    m[0][3] = m[1][3] = m[2][3] = 0;
#define q (Q+3)
    a = q[1] * q[1];
    b = q[2] * q[2];
    c = q[3] * q[3];
    d = q[0] * q[0];

    m[3][3] = (a + b + c + d);	// norm squared of  'unit' quaternion
//	if (q[4] != 0.f)
    m[3][3] *= q[4];	//divide side of projective convention

    m[0][0] = (d + a - b - c);
    m[1][1] = (d + b - a - c);
    m[2][2] = (d + c - a - b);
    c = 2 * q[0];
    a = 2 * q[2] * q[3];
    b = -q[1] * c;
    m[1][2] = (a + b);
    m[2][1] = (a - b);
    a = 2 * q[3] * q[1];
    b = -q[2] * c;
    m[2][0] = (a + b);
    m[0][2] = (a - b);
    a = 2 * q[1] * q[2];
    b = -q[3] * c;
    m[0][1] = (a + b);
    m[1][0] = (a - b);

    m[3][0] = -(Q[0] * m[0][0] + Q[1] * m[1][0] + Q[2] * m[2][0]);
    m[3][1] = -(Q[0] * m[0][1] + Q[1] * m[1][1] + Q[2] * m[2][1]);
    m[3][2] = -(Q[0] * m[0][2] + Q[1] * m[1][2] + Q[2] * m[2][2]);



#undef q
    //	int i, j;
    //	for (i = 4; i--;)for (j = 4; j--;)m[i][j] = -m[i][j];
    glLoadMatrixf((float*)m);
}
#endif

//#pragma warning( disable : 4244)
//#include <windows.h>
//#include <gl/GL.h>
#include <GL/gl.h>
#include "double8.h"
/*
void double8::operator !(){
	glLoadMatrixf((float*)m);	// load inverse of bezel xfm
void double8::operator +(){

// could do constructors from double (*a)[3]  and (*r)[4]  and x,y or z axis rotations by theta
*/

double8::~double8()
{
}


double8::double8()
{
    r[0] = r[1] = r[2] = 0.;
    r[3] = 1.;// cos 2 theta
    r[4] = r[5] = r[6] = 0.;
    r[7] = 1.;// scale
}
double8::double8(double (R)[3])		// from a 3d point
{
    for (int i = 3; i--;)
        r[i] = R[i];
    r[3] = 1.;// cos 2 theta
    r[4] = r[5] = r[6] = 0.;
    r[7] = 1.;// scale

}
/*double8::double8(double(R)[8])		// from a 3d point
{
	for (int i = 8; i--;)r[i] = R[i];
}*/

double8::double8(float *R)
{
    for (int i = 8; i--;)
        r[i] = R[i];
}

// apply this double8 as a gl transform including scale and displacement
void double8::glMult(void) // make bezel view this  in other words multA to GL_MODEL	// 9/23  tested and used
{
    double m[4][4];
    int j;

    double a, b, c, d;

    m[0][3] = m[1][3] = m[2][3] = 0;
#define q (r+3)

    a = q[1] * q[1];
    b = q[2] * q[2];
    c = q[3] * q[3];
    d = q[0] * q[0];

    m[3][3] = (a + b + c + d);	// norm squared of  'unit' quaternion

    m[3][3] /= q[4];	//divide side of projective convention

    for (j = 3; j--;)
        m[3][j] = r[j] * m[3][3];

    m[0][0] = (d + a - b - c);
    m[1][1] = (d + b - a - c);
    m[2][2] = (d + c - a - b);
    c = 2 * q[0];
    a = 2 * q[2] * q[3];
    b = q[1] * c;
    m[1][2] = (a + b);
    m[2][1] = (a - b);
    a = 2 * q[3] * q[1];
    b = q[2] * c;
    m[2][0] = (a + b);
    m[0][2] = (a - b);
    a = 2 * q[1] * q[2];
    b = q[3] * c;
    m[0][1] = (a + b);
    m[1][0] = (a - b);


#undef q

    glMultMatrixd((GLdouble *)m);
}
//	void double8::operator !(){// make bezel view this	// 9/22 tested in program
void double8::operator !() // make bezel view this	// 9/22 tested in program
{

#define Q r
    float m[4][4];
    float a, b, c, d;
    m[0][3] = m[1][3] = m[2][3] = 0;
#define q (Q+3)
    a = q[1] * q[1];
    b = q[2] * q[2];
    c = q[3] * q[3];
    d = q[0] * q[0];

    m[3][3] = (a + b + c + d);	// norm squared of  'unit' quaternion

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

    m[3][0] = -(Q[0] * m[0][0] + Q[1] * m[1][0] + Q[2] * m[2][0]);	// mij already have common multiple with m33. before scale inverse
    m[3][1] = -(Q[0] * m[0][1] + Q[1] * m[1][1] + Q[2] * m[2][1]);
    m[3][2] = -(Q[0] * m[0][2] + Q[1] * m[1][2] + Q[2] * m[2][2]);



#undef q
#undef Q
    //	int i, j;
    //	for (i = 4; i--;)for (j = 4; j--;)m[i][j] = -m[i][j];
    glLoadMatrixf((float*)m);
}
//rotations and scales about a reference point

void double8::from(double3 A, double3 B)
{
    double8::from(A.v, B.v);
}
void double8::from(double8 A, double8 B)
{
    float nn, a, b, c, d;
    float R0, R1, R2;
#define q (r+3)

    a = q[1] * q[1];
    b = q[2] * q[2];
    c = q[3] * q[3];
    d = q[0] * q[0];
    /*	if (q[4] == 0.f)Scale = 1.f; else Scale = exp(0.693147180559945309417*q[4]);		// Log base 2!
    //	Scale =  (q[4] == 0.f) ? 1.f : exp(0.693147180559945309417*q[4]);		// Log base 2!
    nn = 1. / (q[4] * (1.e-23 + a + b + c + d));	// in light of this and the last line, the quaternion is made unit.
    */
    nn = q[4] / (1.e-33 + a + b + c + d);	// in light of this and the last line, the quaternion is unitized.

    R0 = A.r[0] * (d + a - b - c);
    R1 = A.r[1] * (d + b - a - c);
    R2 = A.r[2] * (d + c - a - b);

    a = 2 * q[2] * q[3];
    b = (d = q[1]) * (c = 2 * q[0]);
    R2 += A.r[1] * (a + b);
    R1 += A.r[2] * (a - b);

    a = q[3] * (d = 2 * d);
    b = q[2] * c;
    R0 += A.r[2] * (a + b);
    R2 += A.r[0] * (a - b);

    a = d * q[2];
    b = q[3] * c;
    R1 += A.r[0] * (a + b);
    R0 += A.r[1] * (a - b);// rotate-scale done.

    B.r[0] = r[0] + R0*nn;
    B.r[1] = r[1] + R1*nn;
    B.r[2] = r[2] + R2*nn;
#define AB(A, B, t){		float T[3];\
	T[0] = A[0] * B[0] - A[1] * B[1] - A[2] * B[2] - A[3] * B[3];\
	T[1] = A[0] * B[1] + A[1] * B[0] + A[2] * B[3] - A[3] * B[2];\
	T[2] = A[0] * B[2] + A[2] * B[0] + A[3] * B[1] - A[1] * B[3];\
	t[3] = A[0] * B[3] + A[3] * B[0] + A[1] * B[2] - A[2] * B[1];\
	t[0] = T[0]; t[1] = T[1]; t[2] = T[2];	}

    AB((r + 3), (A.r + 3), (B.r + 3));


#undef q
#undef AB
}

void double8::from(double *t, double *B) 	// 9/22 test in mouse proposed
{
    double nn, a, b, c, d;
    double R0, R1, R2;
#define q (r+3)

    a = q[1] * q[1];
    b = q[2] * q[2];
    c = q[3] * q[3];
    d = q[0] * q[0];
    /*	if (q[4] == 0.f)Scale = 1.f; else Scale = exp(0.693147180559945309417*q[4]);		// Log base 2!
    //	Scale =  (q[4] == 0.f) ? 1.f : exp(0.693147180559945309417*q[4]);		// Log base 2!
    nn = 1. / (q[4] * (1.e-23 + a + b + c + d));	// in light of this and the last line, the quaternion is made unit.
    */
    nn = q[4] / (1.e-33 + a + b + c + d);	// in light of this and the last line, the quaternion is unitized.

    R0 = t[0] * (d + a - b - c);
    R1 = t[1] * (d + b - a - c);
    R2 = t[2] * (d + c - a - b);

    a = 2 * q[2] * q[3];
    b = (d = q[1]) * (c = 2 * q[0]);
    R2 += t[1] * (a + b);
    R1 += t[2] * (a - b);

    a = q[3] * (d = 2 * d);
    b = q[2] * c;
    R0 += t[2] * (a + b);
    R2 += t[0] * (a - b);

    a = d * q[2];
    b = q[3] * c;
    R1 += t[0] * (a + b);
    R0 += t[1] * (a - b);// rotate-scale done.

    B[0] = r[0]+R0*nn;
    B[1] = r[1]+ R1*nn;
    B[2] = r[2]+R2*nn;
#undef q
}

void double8::to(double3 A, double3 B)
{
    double8::to(A.v, B.v);
}
void double8::to(double *T, double *B) // a 3 vector
{
    float nn, a, b, c, d;
    float R0, R1, R2;

    double t[3];
    for (int i = 3; i--;)
        t[i] = T[i] - r[i];// inverse displace
#define q (r+3)

    a = q[1] * q[1];
    b = q[2] * q[2];
    c = q[3] * q[3];
    d = q[0] * q[0];
    /*	if (q[4] == 0.f)Scale = 1.f; else Scale = exp(0.693147180559945309417*q[4]);		// Log base 2!
    //	Scale =  (q[4] == 0.f) ? 1.f : exp(0.693147180559945309417*q[4]);		// Log base 2!
    nn = 1. / (q[4] * (1.e-23 + a + b + c + d));	// in light of this and the last line, the quaternion is made unit.
    */
    nn = 1/ q[4] / (1.e-33 + a + b + c + d);	// in light of this and the last line, the quaternion is unitized.

    R0 = t[0] * (d + a - b - c);
    R1 = t[1] * (d + b - a - c);
    R2 = t[2] * (d + c - a - b);

    a = 2 * q[2] * q[3];
    b = (d = q[1]) * (c = -2 * q[0]);		// -c inverts rotation
    R2 += t[1] * (a + b);
    R1 += t[2] * (a - b);

    a = q[3] * (d = 2 * d);
    b = q[2] * c;
    R0 += t[2] * (a + b);
    R2 += t[0] * (a - b);

    a = d * q[2];
    b = q[3] * c;
    R1 += t[0] * (a + b);
    R0 += t[1] * (a - b);

    B[0] = R0*nn;
    B[1] = R1*nn;
    B[2] = R2*nn;
#undef q
}
extern void glJack(double fix[3], double jacksize);

void double8::gljack(double jacksize) 	// 9/22 tests uniary + operator
{
    glPushMatrix();

    glMult();

#define S .5f
    glBegin(GL_LINES);
    glColor3f(1.f, S,S);
    glVertex3d(jacksize, 0.0f, 0.0f);
    glColor3f(S, .7f, .7f);
    glColor3f(S, 1.f, 1.f);
    glVertex3d(-jacksize, 0.0f, 0.0f);

    glColor3f(S, 1.f, S);
    glVertex3d(0., jacksize, 0.0f);
    glColor3f(.7f,S, .7f);
    glColor3f(1.f, S, 1.f);
    glVertex3d(0.0f, -jacksize, 0.0f);
    //
    glColor3f(S,S, 1.f);
    glVertex3d(0.0f, 0.0f, jacksize);
    glColor3f(.7f, .7f, S);
    glColor3f(1.f, 1.f,S);
    glVertex3d(0.0, 0.0f, -jacksize);
    glEnd();


    glPopMatrix();
}
void double8::gljck(double jacksize) 	// 9/22 tests uniary + operator
{
    glPushMatrix();

    glMult();

    glBegin(GL_LINES);
    glVertex3d(jacksize, 0.0f, 0.0f);
    glVertex3d(-jacksize, 0.0f, 0.0f);

    glVertex3d(0., jacksize, 0.0f);
    glVertex3d(0.0f, -jacksize, 0.0f);
    //
    glVertex3d(0.0f, 0.0f, jacksize);
    glVertex3d(0.0, 0.0f, -jacksize);
    glEnd();


    glPopMatrix();
}
void double8::glblack(double jacksize) 	// 9/22 tests uniary + operator
{
    glPushMatrix();

    glMult();

    glColor3f(0.f, 0.f, 0.f);

    glBegin(GL_LINES);
    glVertex3d(jacksize, 0.0f, 0.0f);
    glVertex3d(-jacksize, 0.0f, 0.0f);

    glVertex3d(0., jacksize, 0.0f);
    glVertex3d(0.0f, -jacksize, 0.0f);
    //
    glVertex3d(0.0f, 0.0f, jacksize);
    glVertex3d(0.0, 0.0f, -jacksize);
    glEnd();


    glPopMatrix();
}

/*  9/22  to be done and tested
void operator +(double8 *);// compose from left to right transform
void operator -(double8 *);// compose with inverse
void operator ^(double8 *);// inverse compose


void double8::operator +(double8 *);// compose from left to right transform (data is rows)
void double8::operator -(double8 *);// compose with inverse
void double8::operator ^(double8 *);// inverse compose

void from(double *A, double *R);
void to(double *d, double *s);

void from(double3, double3);
void to(double3 a, double3 b);


*/

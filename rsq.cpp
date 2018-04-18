

#define MD 13	// more table for double precision 2*24 vs 54

static unsigned short Dtbl[1<<MD];
#define M 13
static unsigned short tbl[(1<<M)];

//extern "C" double sqrt(double);
#include <math.h>
void inv_init(void)
{
    typedef union DL
    {
        double f;
        long long l;
        unsigned long ll[2];
    } DL;
    long i;
    for(i=1<<MD; i--;)
    {
        DL b,c;
        c.f=1.;
        c.ll[1] |= i<<(52-32-MD);
        c.ll[1] |= 1<<(52-32-1-MD);		// add 1/2 lsb

        b.f = 1./c.f;
        b.l += c.l;

        // clear exponent and sign
        Dtbl[i] = (b.ll[1]<< 12)>>( 4+12);		// and fit in 16 bits, not 20
    }
    for(i=1<<M; i--;)
    {
        DL b,c;
        c.f=1.;
        c.ll[1] += i<<(21-M);
        c.ll[1] |= 1<<(20-M);		// add 1/2 lsb

        b.f = 1./sqrt(c.f);
        b.l += (c.l>>1);

        // clear exponent and sign
        tbl[i^(1<<(M-1))] = (b.ll[1]<< 15)>>16;
    }
}
// use inline functions from a .lib??? How can this be?

float inv(float t)
{
    unsigned long l = ((unsigned long *)&t)[0]; // the word of the arg

    union
    {
        float f;
        long l;
    } a;

    if((0x7f800000&l)>0x7e800000l)
        return 0;

    {
        unsigned long index;
//       index =  (l<<(32-23));
//       index >>= (32-MD);
        index = (l>>(23-MD))&((1<<MD)-1) ;
        a.l = (0x7ee00000l | ((unsigned long)Dtbl[index]<<7) )
              - l;

    }
//	a.l = (0x7ee00000l | ((unsigned long)Dtbl[(l<<(32-23))>>(32-MD)]<<7) )
    return  a.f*(2.F- t*a.f);	// a Newton Rafson doubles the significance
}
double inv(double t)
{
    union
    {
        double f;
        long long l;
        long ll[2];
    } b;
    unsigned long l = ((unsigned long *)&t)[1]; // the hi word of the arg

//	b.f =t;
    if((0x7ff00000l&l)>0x7fdc0000l)
        return 0;
    {
        unsigned long index;
        index = (l>>(20-MD) &((1<<MD) - 1) );
//      index = (l>>(23-MD))&(1<<MD-1) ;
        b.ll[1] = ( 0x7fdc0000l|(( long)Dtbl[index]<<4))
                  - l;
    }
    b.ll[0] =0;

    b.f = b.f*(2.-t*b.f);		// a Newton Rafson doubles the significance
    return  b.f *(2.-t*b.f);	// a Newton Rafson doubles the significance
}
float rsq(float t)
{
    union
    {
        float f;
        unsigned long l;
    } b;
    unsigned long l = ((unsigned long *)&t)[0]; // the word of the arg
    int index;
    // i CAN'T ASSUME THAT A LONG IS 32 BITS OR THAT AN ARITHMETIC SHIFT IS NOT >>.
//    index = (l<<(32-24))>>(32-M);
    index = (l>>(24-M))&((1<<M)-1);
    b.l =
        (0x5f300000l |(((unsigned long)tbl[index])<<4))
        - (l>>1);

    return  b.f *(1.5F-.5F*t*b.f*b.f);
}
double rsq(double t)
{
    union
    {
        double f;
        long long  l;
        unsigned long ll[2];
    } b;
    unsigned long l = ((unsigned long *)&t)[1]; // the hi word of the arg
    int index;
    index = (l>>(21-M))&((1<<M)-1);// (l<<(32-21))>>(32-M)



    b.ll[0] =0;
    b.ll[1] =
        (0x5fe60000l |(((unsigned long)tbl[index])<<1))
        - (l>>1);

    b.f = b.f*(1.5-.5*t*b.f*b.f);
    return  b.f *(1.5-.5*t*b.f*b.f);
}
extern "C" {
    float frsq(float t)
    {
        union
        {
            float f;
            unsigned long l;
        } b;
        unsigned long l = ((unsigned long *)&t)[0]; // the word of the arg
        int index;
        index = (l>>(24-M))&((1<<M)-1);// (l<<(32-24))>>(32-M)

        b.l =
            (0x5f300000l |(((unsigned long)tbl[index])<<4))
            - (l>>1);

        return  b.f *(1.5F-.5F*t*b.f*b.f);
    }
}

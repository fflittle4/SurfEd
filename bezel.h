#pragma once
// STUFF defined in bezel.cpp
extern void to_object(float q[8], double *ar, float d[8]);
extern void from_objectd(float q[8], double *ar, double x[3]);
extern void doXkey(void);
extern void doYkey(void);
extern void doZkey(void);
extern void loadA_(float *Q);

typedef struct site{
	struct site *next;
	char *name;
	float q[8];
	long flag;
}SITE;

extern site bezel;

/* A simple program to show how to set up an X window for OpenGL rendering.
 * X86 compilation: gcc -o -L/usr/X11/lib   main main.c -lGL -lX11
 * X64 compilation: gcc -o -L/usr/X11/lib64 main main.c -lGL -lX11
 */
#include <stdio.h>
#include <stdlib.h>

#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>

#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/keysym.h>

//static int snglBuf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};
//static int dblBuf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};

Display   *dpy;
Window     win;
extern GLfloat    xAngle, yAngle, zAngle;
GLboolean  doubleBuffer = GL_TRUE;

extern void startX(int argc, char **argv);
//extern void gluProjection(float r,float l,float t,float b,float n,float f,float s,float xe,float ye,float ze);
extern void zonPaint(void);
extern void zzredraw(void);
extern void handleAnEvent(XEvent *event);
extern void paintit(void);

void fatalError(char *message)
{
    fprintf(stderr, "main: %s\n", message);
    exit(1);
}
#include "rsq.h"
#include <time.h>
#define HRezClock 0
#include "bezel.h"


float xe = 0.f;
float ye = 0.f;
float ze = 2.0f;

int main(int argc, char **argv)
{
    XEvent               event;
#if HRezClock
    struct timespec clock_res,clock_now;
    if(clock_getres(CLOCK_MONOTONIC_RAW,&clock_res ))
    {
        fatalError("no clock res");
    }
#endif


    inv_init();
    startX(argc, argv);
    /*** (8) configure the OpenGL context for rendering ***/

    glEnable(GL_DEPTH_TEST); /* enable depth buffering */
    glDepthFunc(GL_LESS);    /* pedantic, GL_LESS is the default */
    glClearDepth(1.0);       /* pedantic, 1.0 is the default */

    /* frame buffer clears  */
    glClearColor(1.0, 1.0, 1.0, 1.0);

//    gluProjection(-1.f,1.f,-1.f,1.f,1.f,-10.f,0.f,xe,ye,ze);

    while (1)      // This loop happens every 60'th of a second within .001 proportional error.
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        handleAnEvent( 0);    // express 1/60 second.

        /* loop to compress x events */
        //   do
        while(XPending(dpy))
        {
            XNextEvent(dpy, &event);  // blocks if no event pending
            handleAnEvent( &event);
        }

        zonPaint();
        paintit();

        //zzredraw();

        glXSwapBuffers(dpy, win);/* buffer swap does implicit glFlush */
        glFlush();  /* explicit flush for single buffered case */

    }

    return 0;
}
/*
    X events accumulate in mx,my,h,v and in the Hit results
    zonPaint actually changes coordinates of things in the medium from the accumulated intent
    paintit expresses the medium visually

*/

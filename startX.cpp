/* A simple program to show how to set up an X window for OpenGL rendering.
 * X86 compilation: gcc -o -L/usr/X11/lib   main main.c -lGL -lX11
 * X64 compilation: gcc -o -L/usr/X11/lib64 main main.c -lGL -lX11
 12/6 full screen except bottom
 missing f86 stuff.
 */
//#include <stdio.h>
//#include <stdlib.h>

#include <GL/glx.h>    /* this includes the necessary X headers */
//#include <GL/gl.h>

#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
//#include <X11/keysym.h>
#include <X11/extensions/xf86vmode.h>
#include <stdio.h>
#include "rsq.h"
//GLfloat    xAngle = 42.0, yAngle = 82.0, zAngle = 112.0;

void fatalError(char *message);
//void redraw(void);
void fullscreen(Display* dpy, Window win) ;
//void startX(int argc, char **argv);

extern Display   *dpy;
extern Window     win;
extern GLboolean  doubleBuffer;
static int snglBuf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};
static int dblBuf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};
#include "EyeBezel.h"
int	screenHeight,screenWidth;
float iscreenWidth;

//Here's an implementation of what Havoc P suggested, to save the next person the effort:
void startX(int argc, char **argv)
{
    XVisualInfo         *vi;
    Colormap             cmap;
    XSetWindowAttributes swa;
    GLXContext           cx;
    int                  dummy;



    /*** (1) open a connection to the X server ***/

    dpy = XOpenDisplay(NULL);
    if (dpy == NULL)
        fatalError("could not open display");

    XWindowAttributes xwAttr;
    Window wid = DefaultRootWindow( dpy );
    if (  0 > wid )
    {
        fatalError("Failed to obtain the root windows Id "
                   "of the default screen of given display.\n");
    }

//       Status ret =
    XGetWindowAttributes( dpy, wid, &xwAttr );

    screenHeight = xwAttr.height;
    screenWidth = xwAttr.width;  // sum of both monitors
    printf("dpy %dx%d\n",screenWidth,screenHeight);
//       screenHeight = 1080;
    screenWidth = 1920;// oops it is for both screens
    screenHeight = 1080;
    printf("dpy %dx%d\n",screenWidth,screenHeight);

//	iscreenWidth = 1./screenWidth;

    iscreenWidth = inv((float)screenWidth);
   iscreenWidth = 1./screenWidth;

    /*** (2) make sure OpenGL's GLX extension supported ***/

    if(!glXQueryExtension(dpy, &dummy, &dummy))
        fatalError("X server has no OpenGL GLX extension");

    /*** (3) find an appropriate visual ***/

    /* find an OpenGL-capable RGB visual with depth buffer */
    vi = glXChooseVisual(dpy, DefaultScreen(dpy), dblBuf);
    if (vi == NULL)
    {
        vi = glXChooseVisual(dpy, DefaultScreen(dpy), snglBuf);
        if (vi == NULL)
            fatalError("no RGB visual with depth buffer");
        doubleBuffer = GL_FALSE;
    }
    if(vi->c_class != TrueColor)
        fatalError("TrueColor visual required for this program");

    /*** (4) create an OpenGL rendering context  ***/

    /* create an OpenGL rendering context */
    cx = glXCreateContext(dpy, vi, /* no shared dlists */ None,
                          /* direct rendering if possible */ GL_TRUE);
    if (cx == NULL)
        fatalError("could not create rendering context");

    /*** (5) create an X window with the selected visual ***/

    /* create an X colormap since probably not using default visual */
    cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.border_pixel = 0;
    swa.event_mask = KeyPressMask    | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask  | PointerMotionMask |
                     ExposureMask | StructureNotifyMask;


    win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0,
                        300, 300, 0, vi->depth, InputOutput, vi->visual,
                        CWBorderPixel | CWColormap | CWEventMask, &swa);
    XSetStandardProperties(dpy, win, "main", "main", None,
                           argv, argc, NULL);
    fullscreen(dpy, win);

    /*** (6) bind the rendering context to the window ***/

    glXMakeCurrent(dpy, win, cx);

    /*** (7) request the X window to be displayed on the screen ***/

    XMapWindow(dpy, win);


    /*** (8) configure the OpenGL context for rendering ***/



}

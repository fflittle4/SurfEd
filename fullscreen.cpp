/* A simple program to show how to set up an X window for OpenGL rendering.
 * X86 compilation: gcc -o -L/usr/X11/lib   main main.c -lGL -lX11
 * X64 compilation: gcc -o -L/usr/X11/lib64 main main.c -lGL -lX11
 12/6 full screen except bottom
 missing f86 stuff.
 */
#include <stdio.h>
#include <stdlib.h>

#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>

#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/keysym.h>
//#include <X11/extensions/xf86vmode.h>

extern Display   *dpy;
extern Window     win;

#include "EyeBezel.h"
//#include "rsq.h"

//void fatalError(char *message);
void fullscreen(Display* dpy, Window win)
{



    typedef struct
    {
        unsigned long   flags;
        unsigned long   functions;
        unsigned long   decorations;
        long            inputMode;
        unsigned long   status;
    } Hints;



    //IThis structure came from Motif.
    // This structure is required by the XChangeProperty() function which is the key to getting rid of the window borders.
    // You also need an Atom (Atoms are declared in X11/Xatom.h).
    // Don't worry if you don't know what an Atom is, as it's not important knowledge in order to get a borderless window.



    Hints   hints= {0};
    Atom    property;



    //Now with those preliminaries out of the way, we can start to get to the core of what we want: a borderless window. You need to set a couple fields in the structure:



    hints.flags = 2;        // Specify that we're changing the window decorations.

    hints.decorations = 0;  // 0 (false) means that window decorations should go bye-bye.



    //Now we need the Atom that identifies the window manager Motif hints. If you've ever looked at the Xlib header files,
    //you may have seen the XA_WM_HINTS atom that seems to indicate the ability to set window manager hints.
    // Whatever it's used for, setting window manager border hints isn't it. We need to use the "_MOTIF_WM_HINTS" atom instead.



    property = XInternAtom(dpy,"_MOTIF_WM_HINTS",True);

    //The "display" parameter is the X11 Display variable returned from XOpenDisplay, as you may have guessed.
    //If XInternAtom() returns 0, then something went wrong and you should stop here. Otherwise you're ready to get rid of the borders with the following line of code:



    XChangeProperty(dpy,win,property,property,32,PropModeReplace,(unsigned char *)&hints,5);


    XMoveResizeWindow(dpy,win,0,0,screenWidth,screenHeight);
    XMapRaised(dpy,win);
    XGrabPointer(dpy,win,True,0,GrabModeAsync,GrabModeAsync,win,0L,CurrentTime);
    XGrabKeyboard(dpy,win,False,GrabModeAsync,GrabModeAsync,CurrentTime);
    // Add two calls to the extension to get screen dotclock & resolution and set them.

}

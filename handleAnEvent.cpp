#include <stdlib.h>
#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>

#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/keysym.h>
#include <X11/extensions/xf86vmode.h>
#include "mouseProc.h"
#include <stdio.h>    /* X11 constant (e.g. TrueColor) */

int mx, my;
int v=0,h=0;
extern void rmouse(int zero);//,int mx=0,int my=0); // place the integer cursor in 3D and calculate a delta from last call
extern GLboolean            needRedraw, recalcModelView;
extern float    xAngle, yAngle, zAngle;
long tv_res;    // clock tic in nanoseconds
#include "fsm.h"
enum mmode mMode = none;
enum hitreturn nr= HRnothing;
int ns=0,xns = -1;
void EvKeyRelease(KeySym keysym);
void EvKey(KeySym keysym);
#define MACHINE 2
/*
    So Far:
        there are 3  finite state machine:
        1.  translate-dolly-orient-spin based on the left and right mouse buttons for transitions
        2.  What is it on lbd,rbd (and the 'D' key for delete) augmenting bezel for motion
        3.  whate is hit on lbu for merge
        4.  Tick for create/modify  motion kills/extends tick
    pov's
        modify
        hit/drag
        delete/merge
    Xevents from keyboard, mouse, buttons
    drive state
    1.  mouse motion, bezel, bezel-Z drive 3d cursor under 2d mouse cursor.
    2.  buttons down and motion modify bezel position/orientation
    3.  selection of free_point, bezier point ,barb, normals apply 2. to specific medium element.
    3.a 'd' plus selection drives deletion.
    4.  pause for effect (tick) drives creation/promotion
    4.a pause after seg creation makes 'd'
    1.a augment 1. with none of the above after 'd' etc.

*/
//#include "EyeBezel.h"
void  handleAnEvent(XEvent *event)
{
    static fsm *FSM=0;
    if (!FSM)
    {
        FSM = new fsm;
    }

    /*
    #include <time.h>

           CLOCK_MONOTONIC_RAW
           int clock_getres(clockid_t clk_id, struct timespec *res);

           int clock_gettime(clockid_t clk_id, struct timespec *tp);
               struct timespec {
                   time_t   tv_sec;      //   seconds
                   long     tv_nsec;      //  nanoseconds
     */
    if(!event)
    {
        // TICK TOCK
        if(FSM->fsmTIC())
        {
            mMode = waitforLBU;	// something was acted on so don't do anything else!:delete
        };

    }
    else
        switch (event->type)
        {
        case KeyRelease:
        {
            KeySym     keysym;
            char       buffer[1];
            if (   (XLookupString((XKeyEvent *)event,buffer,1,&keysym,NULL) == 1))
            {
                EvKeyRelease(keysym);
            }
        }
        break;

        case KeyPress:
        {
            KeySym     keysym;
            char       buffer[1];

            if (   (XLookupString((XKeyEvent *)event,buffer,1,&keysym,NULL) == 1))
            {
                EvKey(keysym);
            }
        }
        break;

        /* Press-release  of mouse buttons*/
        case ButtonRelease:
            switch (event->xbutton.button)
            {

            case 1:
                switch (mMode)
                {
                case spin:
                    mMode = turn;
                    break;
                case translate:
                    FSM->fsmLBUP();
                    nr = HRnothing;
                case waitforLBU:
                default:
                    mMode = none;
                    break;
                }
                break;

            case 3:
                switch (mMode)
                {
                case dolly:
                    mMode = translate;
                    break;
                case turn:
                default:
                    mMode = none;
                    nr = HRnothing;
                case  waitforLBU:
                    break;
                }
                break;

            case 2:
            default:
                break;
            }
            break;
        case ButtonPress:
            switch(event->xbutton.button)
            {

            case 1:
                switch(mMode)
                {

                case none:
                    mMode = translate;
                    //                   if (MACHINE==0)break;

                    nr = HRnothing;
                    xns = -1;	// hit anything
                    hittest(HCall);
                    printf("hit %3d,%d\n",nr,ns);

                    //                   if (MACHINE==1)break;

                    if(FSM->fsmLBD())
                        mMode = waitforLBU;	// something was acted on so don't do anything else!:delete

                    break;

                case turn:
                    mMode = spin;
                    break;
                default:
                    nr = HRnothing;
                    break;
                }
                break;
            case 3:
                switch (mMode)
                {
                case none:
                    hittest(HCall);
                    mMode = turn;
                    break;
                case translate:
                    mMode = dolly;
                    break;
                default:
                    break;
                }
                break;
            case 2:
                m_destroy = !0;
                break;
            /*  */
            /*
             HEY!  wheels on mouse are handled via fake button presses */
            case 4:
                h++;
                break;
            case 5:
                h--;
                break;
            case 6:
                v++;
                break;
            case 7:
                v--;
                break;
            }
            break;
        /*  */

        case MotionNotify:

            mx = (*(XMotionEvent *)event).x;
            my = (*(XMotionEvent *)event).y;
            FSM->fsmMMove();

            break;

        case ConfigureNotify:
            glViewport(0, 0, event->xconfigure.width, event->xconfigure.height);
        /* fall through... */
        case Expose:

            break;
        default:
            break;
        }
}
/*
    X events propogate no further than this.
   nr,ns, mmode, mx,my,h,v drive onpaint(tranit,rotit) to update media prior to display.
    the keyboard key commands are addressed in onpaint,cpp (keypress/release)
    FSM:
        FSM->fsmTIC()
        FSM->fsmLBD()
        FSM->fsmLBUP();
        FSM->fsmMMove();
            nr,ns,  mx,my,m_destroy

*/


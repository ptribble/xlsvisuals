/*
 * xlsvisuals.c Peter C. Tribble 23/2/2000
 * print information about available visuals. Essentially a
 * stripped down version of xdpyinfo. Original copyright notice:
 *
 * $TOG: xdpyinfo.c /main/35 1998/02/09 13:57:05 kaleb $
 * 
 * xlsvisuals - print information about X display visuals
 *
 * 
Copyright 1988, 1998  The Open Group

All Rights Reserved.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.
 *
 * Author:  Jim Fulton, MIT X Consortium
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <stdlib.h>

char *ProgramName;

static int StrCmp(a, b)
    char **a, **b;
{
    return strcmp(*a, *b);
}

void
print_visual_info (vip)
    XVisualInfo *vip;
{
    char errorbuf[40];			/* for sprintfing into */
    char *class = NULL;			/* for printing */

    switch (vip->class) {
      case StaticGray:   class = "StaticGray "; break;
      case GrayScale:    class = "GrayScale  "; break;
      case StaticColor:  class = "StaticColor"; break;
      case PseudoColor:  class = "PseudoColor"; break;
      case TrueColor:    class = "TrueColor  "; break;
      case DirectColor:  class = "DirectColor"; break;
      default:    
	sprintf (errorbuf, "unknown class %d", vip->class);
	class = errorbuf;
	break;
    }

    printf ("id:    0x%lx    class:    %s    depth:    %d plane%s\n",
	    vip->visualid, class, vip->depth, vip->depth == 1 ? "" : "s");
}

void
print_screen_info (dpy, scr)
    Display *dpy;
    int scr;
{
    Screen *s = ScreenOfDisplay (dpy, scr);  /* opaque structure */
    XVisualInfo viproto;		/* fill in for getting info */
    XVisualInfo *vip;			/* returned info */
    int nvi;				/* number of elements returned */
    int i;				/* temp variable: iterator */
    VisualID defvisid;			/* id of default visual */
    int ndepths = 0, *depths = NULL;


    printf ("\nscreen #%d:\n", scr);
    depths = XListDepths (dpy, scr, &ndepths);
    if (!depths) ndepths = 0;
    printf ("  depths (%d):    ", ndepths);
    for (i = 0; i < ndepths; i++) {
	printf ("%d", depths[i]);
	if (i < ndepths - 1) { 
	    putchar (',');
	    putchar (' ');
	}
    }
    putchar ('\n');
    if (depths) XFree ((char *) depths);
    printf ("  depth of root window:    %d plane%s\n",
	    DisplayPlanes (dpy, scr),
	    DisplayPlanes (dpy, scr) == 1 ? "" : "s");
    printf ("  colormaps:    minimum %d, maximum %d, default 0x%lx\n",
	    MinCmapsOfScreen(s), MaxCmapsOfScreen(s), DefaultColormap (dpy, scr));
    printf ("  default number of colormap cells:    %d\n",
	    DisplayCells (dpy, scr));

    nvi = 0;
    viproto.screen = scr;
    vip = XGetVisualInfo (dpy, VisualScreenMask, &viproto, &nvi);
    defvisid = XVisualIDFromVisual (DefaultVisual (dpy, scr));
    printf ("  number of visuals:    %d,  default id:  0x%lx\n", nvi, defvisid);
    for (i = 0; i < nvi; i++) {
	print_visual_info (vip+i);
    }
    if (vip) XFree ((char *) vip);
}

void
print_default_info (dpy, scr)
    Display *dpy;
    int scr;
{
    Screen *s = ScreenOfDisplay (dpy, scr);  /* opaque structure */
    XVisualInfo viproto;		/* fill in for getting info */
    XVisualInfo *vip;			/* returned info */
    int nvi;				/* number of elements returned */
    int i;				/* temp variable: iterator */
    VisualID defvisid;			/* id of default visual */

    printf ("\nscreen #%d, default visual:\n", scr);

    nvi = 0;
    viproto.screen = scr;
    vip = XGetVisualInfo (dpy, VisualScreenMask, &viproto, &nvi);
    defvisid = XVisualIDFromVisual (DefaultVisual (dpy, scr));
    for (i = 0; i < nvi; i++) {
      if ((vip+i)->visualid == defvisid) {
	print_visual_info (vip+i);
      }
    }
    if (vip) XFree ((char *) vip);
}

void
print_cclass_info (dpy, scr, cclass)
    Display *dpy;
    int scr;
    int cclass;
{
    Screen *s = ScreenOfDisplay (dpy, scr);  /* opaque structure */
    XVisualInfo viproto;		/* fill in for getting info */
    XVisualInfo *vip;			/* returned info */
    int nvi;				/* number of elements returned */
    int i;				/* temp variable: iterator */

    printf ("\nscreen #%d, visuals of class %d:\n", scr, cclass);

    nvi = 0;
    viproto.screen = scr;
    vip = XGetVisualInfo (dpy, VisualScreenMask, &viproto, &nvi);
    for (i = 0; i < nvi; i++) {
      if ((vip+i)->class == cclass) {
	print_visual_info (vip+i);
      }
    }
    if (vip) XFree ((char *) vip);
}

void
print_cdepth_info (dpy, scr, cdepth)
    Display *dpy;
    int scr;
    int cdepth;
{
    Screen *s = ScreenOfDisplay (dpy, scr);  /* opaque structure */
    XVisualInfo viproto;		/* fill in for getting info */
    XVisualInfo *vip;			/* returned info */
    int nvi;				/* number of elements returned */
    int i;				/* temp variable: iterator */

    printf ("\nscreen #%d, visuals of depth %d:\n", scr, cdepth);

    nvi = 0;
    viproto.screen = scr;
    vip = XGetVisualInfo (dpy, VisualScreenMask, &viproto, &nvi);
    for (i = 0; i < nvi; i++) {
      if ((vip+i)->depth == cdepth) {
	print_visual_info (vip+i);
      }
    }
    if (vip) XFree ((char *) vip);
}

static void usage ()
{
    fprintf (stderr, "usage:  %s [options]\n", ProgramName);
    fprintf (stderr, "-display displayname\tquery named server\n");
    fprintf (stderr, "-cc colour_class\tshow visuals this class only\n");
    fprintf (stderr, "-depth depth\t\tshow visuals this depth only\n");
    fprintf (stderr, "-default\t\tshow default visual only\n");
    exit (1);
}

int main (argc, argv)
    int argc;
    char *argv[];
{
    Display *dpy;			/* X connection */
    char *displayname = NULL;		/* server to contact */
    int i;				/* temp variable:  iterator */

    int showdef = 0;    /* show default only */
    int docclass = 0;     /* show this class only */
    int cclass = 0;     /* class to show */
    int dodepth = 0;     /* show this depth only */
    int cdepth = 0;     /* depth to show */

    ProgramName = argv[0];

    for (i = 1; i < argc; i++) {
	char *arg = argv[i];
	int len = strlen(arg);
	
	if (!strncmp("-display", arg, len)) {
	    if (++i >= argc) usage ();
	    displayname = argv[i];
	} else if (!strncmp("-cc", arg, len)) {
	    docclass=1;
	    if (++i >= argc) usage ();
	    cclass = atoi(argv[i]);
	} else if (!strncmp("-depth", arg, len)) {
	    dodepth=1;
	    if (++i >= argc) usage ();
	    cdepth = atoi(argv[i]);
        } else if (!strncmp("-default", arg, len)) {
            showdef=1;
	} else
	    usage ();
    }

    dpy = XOpenDisplay (displayname);
    if (!dpy) {
	fprintf (stderr, "%s:  unable to open display \"%s\".\n",
		 ProgramName, XDisplayName (displayname));
	exit (1);
    }

    if (showdef == 1) {
      for (i = 0; i < ScreenCount (dpy); i++) {
	print_default_info (dpy, i);
      }
      exit (0);
    }

    /*
     * actually only 0 to 5 make sense, but if the user ask for visuals
     * of class 42 then they get nothing, which is sort of expected
     */
    if (docclass == 1) {
      for (i = 0; i < ScreenCount (dpy); i++) {
	print_cclass_info (dpy, i, cclass);
      }
      exit (0);
    }

    if (dodepth == 1) {
      for (i = 0; i < ScreenCount (dpy); i++) {
	print_cdepth_info (dpy, i, cdepth);
      }
      exit (0);
    }

    for (i = 0; i < ScreenCount (dpy); i++) {
	print_screen_info (dpy, i);
    }

    XCloseDisplay (dpy);
    exit (0);
}

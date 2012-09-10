#include <string.h> 
#include <stdio.h> 
#include <GL/glut.h> 
#include <GL/gl.h> 
 

/***********************************************************************************\ 

     W I N D O W S     D E F I N I T I O N S    A N D    V A R I A B L E S
        
\***********************************************************************************/
#define WIDTH  600                                   /* Frame Width is 600 pixels  */
#define HEIGHT 600                                   /* Frame Height is 600 pixels */
#define TITLE  "GLUT Demo: Using Subwindows"         /* Frame Title                */

int winIdMain;                                       /* Main Window handle         */
int winIdSub;                                        /* Sub-Window handle          */
 
int menIdMain;                                       /* Main menu handle           */
int menIdSub;                                        /* Sub-menu handle            */
int menIdSubMain;                                    /* Main menu for sub-window handle */

 


 /***********************************************************************************\ 

     A N I M A T I O N    &   S C E N E   D E F I N I T I O N S  A N D

                              V A R I A B L E S 
        
\***********************************************************************************/
#define SMALL_ANGLE  1.0                            /* Small rotation angle        */
#define SMALL_DISTANCE  0.01                        /* Small translation distance  */
#define TIME_STEP    0.1 

static double rotation_angle = 0.0;                 /* Current angle increment     */
static double translation_distance = 0.0;           /* Current translation increment */

static double time = 0.0;                           /* Current time                */
static double spin = 0.0;                           /* Current Spin Angle          */
static double center = 0.0;                         /* Current Center of Gravity   */

#define RGB_TRIANGLE 1                              /* Call List Objects           */
#define R_TRIANGLE 2
#define G_TRIANGLE 3
#define B_TRIANGLE 4
static int object = RGB_TRIANGLE;                   /* Object to use in scene      */

 


/***********************************************************************************\ 

     F U N C T I O N S    &    V A R I A B L E S    F O R   S T R I N G 
 
                                R A S T E R I N G
        
\***********************************************************************************/
static char label[100];                            /* Storage for current string   */
 

/* Draw the  string *s one character at a time using GLUT's bitmap
   rendering utility  */
void  
drawString (char *s) 
{ 
  unsigned int i; 
  for (i = 0; i < strlen (s); i++) 
    glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, s[i]); 
}; 
 

/*  Same as before but using a larger font bitmap   */
void  
drawStringBig (char *s) 
{ 
  unsigned int i; 
  for (i = 0; i < strlen (s); i++) 
    glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, s[i]); 
}; 
 





/***********************************************************************************\ 

        W I N D O W S   U P D A T I N G   C A L L B A C K   F U N C T I O N S
        
\***********************************************************************************/
void  
mainDisplay (void) 
{ 
 
  /* Clean drawing board */ 
  glutSetWindow (winIdMain); 
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  glLoadIdentity (); 
 
  /* Write Footnote */ 
  glColor3f (1.0F, 1.0F, 1.0F); 
  sprintf (label, "(c)Miguel Angel Sepulveda 1998"); 
  glRasterPos2f (-0.80F, -0.80F); 
  drawString (label); 
  
  /* Draw rotating triangle */ 
  glTranslatef(center, 0.0, 0.0);
  glPushMatrix (); 
  glRotatef (spin, 0.0, 0.0, 1.0); 
  glCallList(object);
  glPopMatrix (); 
 
  glutSwapBuffers (); 
}; 
 
 

/* Another display function, this one will be  
   used to update the graphic subwindow */ 
void  
subDisplay () 
{ 
 
  /* Clear subwindow */ 
  glutSetWindow (winIdSub); 
  glClearColor (0.25, 0.25, 0.25, 0.0); 
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
 
  /* Draw border */ 
  glColor3f (0.0F, 1.0F, 0.0F); 
  glBegin (GL_LINE_LOOP); 
  glVertex2f (0.0F, 0.0F); 
  glVertex2f (0.0F, 0.99F); 
  glVertex2f (0.999F, 0.99F); 
  glVertex2f (0.999F, 0.0F); 
  glEnd (); 
 

  /* Write State Variables */ 
  glColor3f (1.0F, 1.0F, 1.0F); 
  sprintf (label, "Time = %8.3f ", time); 
  glRasterPos2f (0.05F, 0.75F); 
  drawString (label); 
 
  sprintf (label, "Rotation Angle = %8.3f ", spin); 
  glRasterPos2f (0.05F, 0.55F); 
  drawString (label); 
 
  sprintf (label, "Rotation Velocity = %8.3f ", spin / time); 
  glRasterPos2f (0.05F, 0.35F); 
  drawString (label); 
 

  /* Write Banner and info */ 
  glColor3f (1.0F, 0.0F, 1.0F); 
  sprintf (label, " This is a subwindow "); 
  glRasterPos2f (0.40F, 0.70F); 
  drawStringBig (label); 
 
  sprintf (label, " It has its own OpenGL context "); 
  glRasterPos2f (0.33F, 0.35F); 
  drawStringBig (label); 
 
  glutSwapBuffers (); 
}; 


/* Callback function for reshaping the main window */ 
void  
mainReshape (int w, int h) 
{ 
  /* Viewing and Projection transformations */
  glViewport (0, 0, w, h); 
  glMatrixMode (GL_PROJECTION); 
  glLoadIdentity (); 
  gluOrtho2D (-1.0F, 1.0F, -1.0F, 1.0F); 
  glMatrixMode (GL_MODELVIEW); 
  glLoadIdentity (); 
 
  /* Invoke subwindow reshape */
  glutSetWindow (winIdSub); 
  glutReshapeWindow (w - 10, h / 10); 
  glutPositionWindow (5, 5); 
  glutSetWindow (winIdMain); 
 
}; 
 

/* Callback function for reshaping the subwindow */ 
void  
subReshape (int w, int h) 
{ 
  glViewport (0, 0, w, h); 
  glMatrixMode (GL_PROJECTION); 
  glLoadIdentity (); 
  gluOrtho2D (0.0F, 1.0F, 0.0F, 1.0F); 
}; 
 






/***********************************************************************************\ 

        K E Y B O A R D    I N P U T   H A N D L I N G 
        
  Active Keys : i, I, q, Q

      i, I       switch on and off the info banner
      q, Q       quit the program

  Note: parameters  x and y correspond to the current location of the mouse
                   and they are not used in the program

\***********************************************************************************/
/* Now comes a function that processes keyboard events */ 
void  
keyboard (unsigned char key, int x, int y) 
{ 
  static int info_banner = 1; 
 
  switch (key) 
    { 
    case 'i': 
    case 'I': 
      if (info_banner) 
	{ 
	  glutSetWindow (winIdSub); 
	  glutHideWindow (); 
	} 
      else 
	{ 
	  glutSetWindow (winIdSub); 
	  glutShowWindow (); 
	}; 
      info_banner = !info_banner; 
      break; 
    case 'q': 
    case 'Q': 
      exit (0); 
      break; 
    }; 
}; 
 
 
/***********************************************************************************\ 

        M E N U    E V E N T S    H A N D L I N G
        
\***********************************************************************************/
void
mainMenu(int value){

  static int is_rotation = 0;        
  static int is_translation = 0;

  switch(value){

  case 1:

    if(is_rotation) {
      rotation_angle = 0.0;
      is_rotation = 0;
      glutChangeToMenuEntry(1, "Rotation On", 1);
    }
    else {
      rotation_angle = SMALL_ANGLE;
      is_rotation = 1;
     glutChangeToMenuEntry(1, "Rotation Off", 1);
     };

    break;

  case 2:

    if(is_translation) {
      translation_distance = 0.0;
      is_translation = 0;
      glutChangeToMenuEntry(2, "Translation On", 2);
     }
    else {
      translation_distance = SMALL_DISTANCE;
      is_translation = 1;
      glutChangeToMenuEntry(2, "Translation Off", 2);
    };

    break;

  case 4:

    exit(0);
    break;

  };

};


void
subMenu(int value){

  switch(value){

  case 1:
    object = R_TRIANGLE;
    break;

  case 2:
    object = B_TRIANGLE;
    break;

  case 3:
    object = G_TRIANGLE;
    break;

  case 4:
    object = RGB_TRIANGLE;
    break;
  };

};

void dummy(int value){};
 
 
/***********************************************************************************\ 

                               A N I M A T I O N     
        
\***********************************************************************************/

/* There can be only one idle() callback function. In an 
   animation, this idle() function must update not only the 
   main window but also all derived subwindows */ 
void  
idle (void) 
{ 
 
  /* Update  state variables */ 
  time += TIME_STEP; 
  spin += rotation_angle; 
  center += translation_distance;
  if(center > 1.0) center -= 2.0;
 
  /* Update main and sub window */ 
  glutSetWindow (winIdMain); 
  glutPostRedisplay (); 
  glutSetWindow (winIdSub); 
  glutPostRedisplay (); 
}; 


 
/***********************************************************************************\ 

    M A I N :  Initializes GLUT machine, opens frames, windows and menus and
                              runs GLUT's  main loop                         
        
         IMPORTANT NOTE:  Pay attention to the order of the elements


\***********************************************************************************/
int  
main (int argc, char *argv[]) 
{ 
 
  /* Glut initializations */ 
  glutInit (&argc, argv); 
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); 
  glutInitWindowPosition (5, 5); 
  glutInitWindowSize (WIDTH, HEIGHT); 
 

  /* Main window creation and setup */ 
  winIdMain = glutCreateWindow (TITLE); 
  glutDisplayFunc (mainDisplay); 
  glutReshapeFunc (mainReshape); 
  glutKeyboardFunc (keyboard); 
  glutIdleFunc (idle); 

 
  /* Create submenu */
  menIdSub = glutCreateMenu(subMenu);
  glutAddMenuEntry("Red", 1);
  glutAddMenuEntry("Blue", 2);
  glutAddMenuEntry("Green", 3);
  glutAddMenuEntry("RGB", 4); 


  /* Create main menu */
  menIdMain = glutCreateMenu(mainMenu);
  glutAddMenuEntry("Rotation On", 1);
  glutAddMenuEntry("Translation On", 2);
  glutAddSubMenu("Change Color", menIdSub);
  glutAddMenuEntry("Quit", 4);
  glutAttachMenu(GLUT_LEFT_BUTTON);


 /* Create call list elements  */
  glNewList(RGB_TRIANGLE, GL_COMPILE);
  glBegin (GL_POLYGON); 
  glColor3f (1.0F, 0.0F, 0.0F); 
  glVertex2f (0.0F, 0.5F); 
  glColor3f (0.0F, 1.0F, 0.0F); 
  glVertex2f (-0.4330F, -0.25F); 
  glColor3f (0.0F, 0.0F, 1.0F); 
  glVertex2f (0.433F, -0.25F); 
  glEnd (); 
  glEndList();


  glNewList(R_TRIANGLE, GL_COMPILE);
  glBegin (GL_POLYGON); 
  glColor3f (1.0F, 0.0F, 0.0F); 
  glVertex2f (0.0F, 0.5F); 
  glVertex2f (-0.4330F, -0.25F); 
  glVertex2f (0.433F, -0.25F); 
  glEnd (); 
  glEndList();


  glNewList(G_TRIANGLE, GL_COMPILE);
  glBegin (GL_POLYGON); 
  glColor3f (0.0F, 1.0F, 0.0F); 
  glVertex2f (0.0F, 0.5F); 
  glVertex2f (-0.4330F, -0.25F); 
  glVertex2f (0.433F, -0.25F); 
  glEnd (); 
  glEndList();


  glNewList(B_TRIANGLE, GL_COMPILE);
  glBegin (GL_POLYGON); 
  glColor3f (0.0F, 0.0F, 1.0F); 
  glVertex2f (0.0F, 0.5F); 
  glVertex2f (-0.4330F, -0.25F); 
  glVertex2f (0.433F, -0.25F); 
  glEnd (); 
  glEndList();


  /* Sub window creation and setup */ 
  winIdSub = glutCreateSubWindow (winIdMain, 5, 5, WIDTH - 10, HEIGHT / 10); 
  glutDisplayFunc (subDisplay); 
  glutReshapeFunc (subReshape); 


  /* Create main menu for subwindow */
  menIdSubMain = glutCreateMenu(dummy);
  glutAddMenuEntry("This is a dummy menu", 1);
  glutAttachMenu(GLUT_LEFT_BUTTON);

  glutMainLoop (); 
 
  return 0; 
}; 




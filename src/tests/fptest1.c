/* Test GL_NV_fragment_program */

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>



static void Display( void )
{
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   glPushMatrix();

   glColor4f(0, 0.5, 0, 1);
   glBegin(GL_POLYGON);
   glVertex2f(-1, -1);
   glVertex2f( 1, -1);
   glVertex2f( 0,  1);
   glEnd();

   glPopMatrix();

   glutSwapBuffers();
}


static void Reshape( int width, int height )
{
   glViewport( 0, 0, width, height );
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   glFrustum( -1.0, 1.0, -1.0, 1.0, 5.0, 25.0 );
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();
   glTranslatef( 0.0, 0.0, -15.0 );
}


static void Key( unsigned char key, int x, int y )
{
   (void) x;
   (void) y;
   switch (key) {
      case 27:
         exit(0);
         break;
   }
   glutPostRedisplay();
}


static void Init( void )
{
   static const char *prog1 =
      "!!FP1.0\n"
      "MUL   o[COLR], R0, f[WPOS]; \n"
      "ADD   o[COLH], H3, f[TEX0]; \n"
      "ADD_SAT o[COLH], H3, f[TEX0]; \n"
      "ADDX o[COLH], H3, f[TEX0]; \n"
      "ADDHC o[COLH], H3, f[TEX0]; \n"
      "ADDXC o[COLH], H3, f[TEX0]; \n"
      "ADDXC_SAT o[COLH], H30, f[TEX0]; \n"
      "MUL   o[COLR].xy, R0.wzyx, f[WPOS]; \n"
      "MUL   o[COLR], H0, f[WPOS]; \n"
      "MUL   o[COLR], -H0, f[WPOS]; \n"
      "MOV   RC, H1; \n"
      "MOV   HC, H2; \n"
      ;

   /* masked updates */
   static const char *prog2 =
      "!!FP1.0\n"
      "DEFINE foo = {1, 2, 3, 4}; \n"
      "DEFINE foo2 = 5; \n"
      "DECLARE foo = {5, 6, 7, 8}; \n"
      "DECLARE bar = 3; \n"
      "DECLARE bar2; \n"
      "DECLARE bar3 = bar; \n"
      "DECLARE bar4 = { a, b, c, d }; \n"
      "MOV o[COLR],      R0; \n"
      "MOV o[COLR].xy,   R0; \n"
      "MOV o[COLR] (NE), R0; \n"
      "MOV o[COLR] (NE.wzyx), R0; \n"
      "MOV o[COLR].xy (NE.wzyx), R0; \n"
      "KIL NE; \n"
      "KIL EQ.xyxy; \n"
      ;

   /* double the color */
   static const char *prog3 =
      "!!FP1.0\n"
      "MOV R0, f[COL0]; \n"
      "ADD o[COLR], R0, f[COL0]; \n"
      ;

   GLuint progs[20];

   glGenProgramsNV(20, progs);
   assert(progs[0]);
   assert(progs[1]);
   assert(progs[0] != progs[1]);

   glGenProgramsNV(3, progs + 2);
   assert(progs[2]);
   assert(progs[3]);
   assert(progs[2] != progs[3]);
   assert(progs[0] != progs[2]);

   glLoadProgramNV(GL_FRAGMENT_PROGRAM_NV, progs[0],
                   strlen(prog1),
                   (const GLubyte *) prog1);
   assert(glIsProgramNV(progs[0]));

   glLoadProgramNV(GL_FRAGMENT_PROGRAM_NV, progs[1],
                   strlen(prog2),
                   (const GLubyte *) prog2);
   assert(glIsProgramNV(progs[1]));

   glLoadProgramNV(GL_FRAGMENT_PROGRAM_NV, progs[2],
                   strlen(prog2),
                   (const GLubyte *) prog3);
   assert(glIsProgramNV(progs[2]));
   glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, progs[2]);
   glEnable(GL_FRAGMENT_PROGRAM_NV);
   glEnable(GL_ALPHA_TEST);
   glAlphaFunc(GL_ALWAYS, 0.0);

   printf("glGetError = %d\n", (int) glGetError());
}


int main( int argc, char *argv[] )
{
   glutInit( &argc, argv );
   glutInitWindowPosition( 0, 0 );
   glutInitWindowSize( 250, 250 );
   glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
   glutCreateWindow(argv[0]);
   glutReshapeFunc( Reshape );
   glutKeyboardFunc( Key );
   glutDisplayFunc( Display );
   Init();
   glutMainLoop();
   return 0;
}

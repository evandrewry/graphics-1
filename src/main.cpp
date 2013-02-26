//#elif defined(__linux)
#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include "GLScreenCapturer.h"
#include "trackball.h"
#include "lego/lego.h"

using namespace std;

#define BUFFER_LENGTH 64

GLfloat camRotX, camRotY, camPosX, camPosY, camPosZ;
GLint viewport[4];
GLdouble modelview[16];
GLdouble projection[16];

GLuint pickedObj = -1;
GLuint legoDL;

char titleString[150];

bool isTeapot1_selected = false;
bool isTeapot2_selected = false;

// Lights & Materials
GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
GLfloat position[] = {200, 200, 200, 100};
GLfloat position2[] = {-200, -200, -200, -100};
GLfloat mat_diffuse[] = {0.6, 0.6, 0.6, 1.0};
GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_shininess[] = {20.0};
GLfloat global_ambient[] = { 0.4, 0.4, 0.4, 1 };


static GLScreenCapturer screenshot("screenshot-%d.ppm");

void initLights(void)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, mat_diffuse);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular);

    //glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    //glLightfv(GL_LIGHT1, GL_POSITION, position2);
    //glLightfv(GL_LIGHT1, GL_DIFFUSE, mat_diffuse);
    //glLightfv(GL_LIGHT1, GL_SPECULAR, mat_specular);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    //glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void init()
{
    tbInit(GLUT_RIGHT_BUTTON);
    tbAnimate(GL_TRUE);

    // Place Camera
    camRotX = 40.0f;
    camRotY = 40.0f;
    camPosX = 0.0f;
    camPosY = 0.0f;
    camPosZ = -100.0f;

    glEnable( GL_DEPTH_TEST );
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    initLights();

    legoDL = glGenLists (1);
    glNewList(legoDL, GL_COMPILE);
    {
        GLfloat color[] = {0, 1, 0, 1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);

        multilego();
    }
    glEndList();

    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void setCamera( void )
{
    glTranslatef(0, 0, camPosZ);
    glRotatef(camRotX, 1, 0, 0);
    glRotatef(camRotY, 0, 1, 0);
}

void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glPushMatrix();
    {
        setCamera();
        tbMatrix();

        glCallList(legoDL);

        // Retrieve current matrice before they popped.
        glGetDoublev( GL_MODELVIEW_MATRIX, modelview );        // Retrieve The Modelview Matrix
        glGetDoublev( GL_PROJECTION_MATRIX, projection );    // Retrieve The Projection Matrix
        glGetIntegerv( GL_VIEWPORT, viewport );                // Retrieves The Viewport Values (X, Y, Width, Height)
    }
    glPopMatrix();

    glFlush();
    // End Drawing calls
    glutSwapBuffers();
}

void reshape( int w, int h )
{
    tbReshape(w, h);

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the clipping volume
    gluPerspective(60.0f, (GLfloat)w / (GLfloat)h, 1.0f, 500.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void keyboard( unsigned char key, int x, int y )
{
    switch(key)
    {
        case 27: // Escape key
            exit(0);
            break;
        case 'x':
        case 101: //up arrow key
            glRotatef(1.,1.0,0.0,0.0);
            break;
        case 'X':
        case 103: //down arrow key
            glRotatef(-1.,1.0,0.0,0.0);
            break;
        case 'z':
        case 100: //left arrow key
            camPosZ++;
            glutPostRedisplay();
            break;
        case 'Z':
        case 102:
            camPosZ--;
            glutPostRedisplay();
            break;
        case 'y':
        case '/':
            glRotatef(1.,0.0,1.0,1.0);
            break;
        case 'Y':
        case '.':
            glRotatef(-1.,0.0,1.0,1.0);
            break;
        case 'i':
        case 'I':
            glLoadIdentity();
            gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);
            break;
        case 'r':
            printf("save current screen\n");
            screenshot.capture();
            return;
    }
    glutPostRedisplay();
}

void mouse( int button, int state, int x, int y)
{
    tbMouse(button, state, x, y);
}

void motion(int x, int y)
{
    tbMotion(x, y);

    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
    glutPostRedisplay();

}

int main (int argc, char *argv[])
{
    int win_width = 512;
    int win_height = 512;

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize( win_width, win_height );

    glutCreateWindow( "lego shit" );
    init();

    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mouse );
    glutMotionFunc( motion );

    glutMainLoop();
}

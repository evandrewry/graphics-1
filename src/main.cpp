/* Simple opengl demo program. 
 */

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

#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))

using namespace std;

#define BUFFER_LENGTH 64
#define LEGO_HEIGHT 10
#define LEGO_WIDTH 10
#define WALL_WIDTH 2
#define LEGO_LENGTH 30

GLfloat camRotX, camRotY, camPosX, camPosY, camPosZ;
GLint viewport[4];
GLdouble modelview[16];
GLdouble projection[16];

GLuint pickedObj = -1;
GLuint theLego;

char titleString[150];

bool isTeapot1_selected = false;
bool isTeapot2_selected = false;

// Lights & Materials
GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
GLfloat position[] = {0.0, 0.0, 2.0, 1.0};
GLfloat mat_diffuse[] = {0.6, 0.6, 0.6, 1.0};
GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_shininess[] = {50.0};

static GLScreenCapturer screenshot("screenshot-%d.ppm");

void initLights(void)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

//LEGO VERTICES

//main eight
static GLfloat vertex_a[3] = {-LEGO_LENGTH/2, -LEGO_WIDTH/2, 0};
static GLfloat vertex_b[3] = {-LEGO_LENGTH/2, -LEGO_WIDTH/2, LEGO_HEIGHT};
static GLfloat vertex_c[3] = {LEGO_LENGTH/2, -LEGO_WIDTH/2, 0};
static GLfloat vertex_d[3] = {LEGO_LENGTH/2, -LEGO_WIDTH/2, LEGO_HEIGHT};
static GLfloat vertex_e[3] = {LEGO_LENGTH/2, LEGO_WIDTH/2, 0};
static GLfloat vertex_f[3] = {LEGO_LENGTH/2, LEGO_WIDTH/2, LEGO_HEIGHT};
static GLfloat vertex_g[3] = {-LEGO_LENGTH/2, LEGO_WIDTH/2, 0};
static GLfloat vertex_h[3] = {-LEGO_LENGTH/2, LEGO_WIDTH/2, LEGO_HEIGHT};

static GLfloat *lego_side_vertices[8] = {
    &vertex_a[0], &vertex_b[0],
    &vertex_c[0], &vertex_d[0],
    &vertex_e[0], &vertex_f[0],
    &vertex_g[0], &vertex_h[0]
};

static GLfloat inner_vertex_a[3] = {-(LEGO_LENGTH/2 - WALL_WIDTH), -(LEGO_WIDTH/2 - WALL_WIDTH), 0};
static GLfloat inner_vertex_ag[3] = {-LEGO_LENGTH/2, -(LEGO_WIDTH/2 - WALL_WIDTH), 0};
static GLfloat inner_vertex_ac[3] = {-(LEGO_LENGTH/2 - WALL_WIDTH), -LEGO_WIDTH/2, 0};
static GLfloat inner_vertex_c[3] = {(LEGO_LENGTH/2 - WALL_WIDTH), -(LEGO_WIDTH/2 - WALL_WIDTH), 0};
static GLfloat inner_vertex_ca[3] = {(LEGO_LENGTH/2 - WALL_WIDTH), -LEGO_WIDTH/2, 0};
static GLfloat inner_vertex_ce[3] = {LEGO_LENGTH/2, -(LEGO_WIDTH/2 - WALL_WIDTH), 0};
static GLfloat inner_vertex_e[3] = {(LEGO_LENGTH/2 - WALL_WIDTH), (LEGO_WIDTH/2 - WALL_WIDTH), 0};
static GLfloat inner_vertex_ec[3] = {LEGO_LENGTH/2, (LEGO_WIDTH/2 - WALL_WIDTH), 0};
static GLfloat inner_vertex_eg[3] = {(LEGO_LENGTH/2 - WALL_WIDTH), LEGO_WIDTH/2, 0};
static GLfloat inner_vertex_g[3] = {-(LEGO_LENGTH/2 - WALL_WIDTH), (LEGO_WIDTH/2 - WALL_WIDTH), 0};
static GLfloat inner_vertex_ge[3] = {-(LEGO_LENGTH/2 - WALL_WIDTH), LEGO_WIDTH/2, 0};
static GLfloat inner_vertex_ga[3] = {-LEGO_LENGTH/2, (LEGO_WIDTH/2 - WALL_WIDTH), 0};
static GLfloat inner_vertex_b[3] = {-(LEGO_LENGTH/2 - WALL_WIDTH), -(LEGO_WIDTH/2 - WALL_WIDTH), LEGO_HEIGHT - WALL_WIDTH};
static GLfloat inner_vertex_bd[3] = {-(LEGO_LENGTH/2 - WALL_WIDTH), -LEGO_WIDTH/2, LEGO_HEIGHT - WALL_WIDTH};
static GLfloat inner_vertex_bh[3] = {-LEGO_LENGTH/2, -(LEGO_WIDTH/2 - WALL_WIDTH), LEGO_HEIGHT - WALL_WIDTH};
static GLfloat inner_vertex_h[3] = {-(LEGO_LENGTH/2 - WALL_WIDTH), (LEGO_WIDTH/2 - WALL_WIDTH), LEGO_HEIGHT - WALL_WIDTH};
static GLfloat inner_vertex_hd[3] = {-(LEGO_LENGTH/2 - WALL_WIDTH), -LEGO_WIDTH/2, LEGO_HEIGHT - WALL_WIDTH};
static GLfloat inner_vertex_hh[3] = {-LEGO_LENGTH/2, -(LEGO_WIDTH/2 - WALL_WIDTH), LEGO_HEIGHT - WALL_WIDTH};
static GLfloat inner_vertex_d[3] = {(LEGO_LENGTH/2 - WALL_WIDTH), -(LEGO_WIDTH/2 - WALL_WIDTH), LEGO_HEIGHT - WALL_WIDTH};
static GLfloat inner_vertex_db[3] = {(LEGO_LENGTH/2 - WALL_WIDTH), -LEGO_WIDTH/2, LEGO_HEIGHT - WALL_WIDTH};
static GLfloat inner_vertex_df[3] = {LEGO_LENGTH/2, -(LEGO_WIDTH/2 - WALL_WIDTH), LEGO_HEIGHT - WALL_WIDTH};
static GLfloat inner_vertex_f[3] = {(LEGO_LENGTH/2 - WALL_WIDTH), (LEGO_WIDTH/2 - WALL_WIDTH), LEGO_HEIGHT - WALL_WIDTH};
static GLfloat inner_vertex_fh[3] = {(LEGO_LENGTH/2 - WALL_WIDTH), LEGO_WIDTH/2, LEGO_HEIGHT - WALL_WIDTH};
static GLfloat inner_vertex_fd[3] = {LEGO_LENGTH/2, (LEGO_WIDTH/2 - WALL_WIDTH), LEGO_HEIGHT - WALL_WIDTH};

static GLfloat *lego_base_corner_vertices[4][4] = {
    { &inner_vertex_a[0], &inner_vertex_ac[0], &vertex_a[0], &inner_vertex_ag[0] },
    { &inner_vertex_c[0], &inner_vertex_ce[0], &vertex_c[0], &inner_vertex_ca[0] },
    { &inner_vertex_e[0], &inner_vertex_eg[0], &vertex_e[0], &inner_vertex_ec[0] },
    { &inner_vertex_g[0], &inner_vertex_ga[0], &vertex_g[0], &inner_vertex_ge[0] }
};

static GLfloat *lego_base_edge_vertices[4][4] = {
    { &inner_vertex_a[0], &inner_vertex_ac[0], &inner_vertex_ca[0], &inner_vertex_c[0] },
    { &inner_vertex_c[0], &inner_vertex_ce[0], &inner_vertex_ec[0], &inner_vertex_e[0] },
    { &inner_vertex_e[0], &inner_vertex_eg[0], &inner_vertex_ge[0], &inner_vertex_g[0] },
    { &inner_vertex_g[0], &inner_vertex_ga[0], &inner_vertex_ag[0], &inner_vertex_a[0] }
};

static void quad_cycle(GLfloat *verts[])
{
    int i;
    glBegin(GL_QUAD_STRIP);
    {
        for (i = 0; i < 8; i++){
            glVertex3fv(&verts[i][0]);
        }
        glVertex3fv(&verts[0][0]);
        glVertex3fv(&verts[1][0]);
    }
    glEnd();

}
static GLfloat *lego_inner_face_vertices[8] = {
    &inner_vertex_a[0], &inner_vertex_b[0], &inner_vertex_c[0], &inner_vertex_d[0],
    &inner_vertex_e[0], &inner_vertex_f[0], &inner_vertex_g[0], &inner_vertex_h[0]
};

static void lego_sides(){
    quad_cycle(lego_side_vertices);
}

static void lego_inner_faces(){
    quad_cycle(lego_inner_face_vertices);
}

static void lego_base_edges(){
    int i, j;
    glBegin(GL_QUADS);
    {
        for (i = 0; i < 4; i++){
            for (j = 0; j < 4; j++){
                glVertex3fv(&lego_base_edge_vertices[i][j][0]);
            }
        }
    }
    glEnd();
}


static void lego_base_corners()
{
    int i, j;
    for (i = 0; i < 4; i++) {
        glBegin(GL_TRIANGLE_FAN);
        {
            for (j = 0; j < 4; j++) {
                glVertex3fv(&lego_base_corner_vertices[i][j][0]);
            }
        }
        glEnd();
    }
}

static void lego_base()
{
    lego_base_corners();
    lego_base_edges();
}


static void lego()
{
    lego_sides();
    lego_base();
    lego_inner_faces();
}

void init()
{
    tbInit(GLUT_RIGHT_BUTTON);
    tbAnimate(GL_TRUE);

    // Place Camera
    camRotX = 350.0f;
    camRotY = 680.0f;
    camPosX = 0.0f;
    camPosY = 0.0f;
    camPosZ = -80.0f;

    glEnable( GL_DEPTH_TEST );
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    initLights();

    theLego = glGenLists (1);
    glNewList(theLego, GL_COMPILE);
    lego();
    glEndList();

    glShadeModel(GL_FLAT);
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void setCamera( void )
{
    glTranslatef(0, 0, camPosZ);
    glRotatef(camRotX, 1, 0, 0);
    glRotatef(camRotY, 0, 1, 0);
}

void drawSelectableTeapots( void )
{
    float currentColor[4];
    glGetFloatv(GL_CURRENT_COLOR, currentColor);

    GLfloat selectedColor[] = {0, 1, 0, 1};
    GLfloat unselectedColor[] = {1, 0, 0, 1};

    // Initialize the name stack
    glInitNames();
    glPushName(0);

    // Draw two teapots next to each other in z axis
    glPushMatrix();
    {

        if( isTeapot1_selected )
            glMaterialfv(GL_FRONT, GL_DIFFUSE, selectedColor);
        else
            glMaterialfv(GL_FRONT, GL_DIFFUSE, unselectedColor);
        glLoadName(0);
        glutSolidTeapot(2.5);

        if( isTeapot2_selected )
            glMaterialfv(GL_FRONT, GL_DIFFUSE, selectedColor);
        else
            glMaterialfv(GL_FRONT, GL_DIFFUSE, unselectedColor);
        glLoadName(1);
        glTranslatef(0,0,5);
        glutSolidTeapot(1.5);
    }
    glPopMatrix();

    glColor4fv(currentColor);

}

void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glPushMatrix();
    {
        setCamera();
        tbMatrix();

        glCallList(theLego);

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
    gluPerspective(30.0f, (GLfloat)w / (GLfloat)h, 1.0f, 100.0f);

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
            glRotatef(1.,0.0,0.0,1.0);
            break;
        case 'Z':
        case 102:
            glRotatef(-1.,0.0,0.0,1.0);
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
            gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
            break;
        case 'r':
            printf("save current screen\n");
            screenshot.capture();
            return;
    }
    glutPostRedisplay();
}

void processSelection(int xPos, int yPos)
{
    GLfloat fAspect;

    // Space for selection buffer
    static GLuint selectBuff[BUFFER_LENGTH];

    // Hit counter and viewport storage
    GLint hits, viewport[4];

    // Setup selection buffer
    glSelectBuffer(BUFFER_LENGTH, selectBuff);

    // Get the viewport
    glGetIntegerv(GL_VIEWPORT, viewport);

    // Switch to projection and save the matrix
    glMatrixMode(GL_PROJECTION);

    glPushMatrix();
    {
        // Change render mode
        glRenderMode(GL_SELECT);

        // Establish new clipping volume to be unit cube around
        // mouse cursor point (xPos, yPos) and extending two pixels
        // in the vertical and horizontal direction
        glLoadIdentity();
        gluPickMatrix(xPos, viewport[3] - yPos + viewport[1], 0.1,0.1, viewport);

        // Apply perspective matrix 
        fAspect = (float)viewport[2] / (float)viewport[3];
        gluPerspective(45.0f, fAspect, 1.0, 425.0);


        // Render only those needed for selection
        glPushMatrix();    
        {
            setCamera();
            tbMatrixForSelection();
            glCallList(theLego); 

        }
        glPopMatrix();


        // Collect the hits
        hits = glRenderMode(GL_RENDER);

        isTeapot1_selected = false;
        isTeapot2_selected = false;

        // If hit(s) occurred, display the info.
        if(hits != 0)
        {

            // Save current picked object.
            // Take only the nearest selection
            pickedObj = selectBuff[3];

            sprintf (titleString, "You clicked on %d", pickedObj);
            glutSetWindowTitle(titleString);

            if (pickedObj == 0) {
                isTeapot1_selected = true;
            }

            if (pickedObj == 1) {
                isTeapot2_selected = true;
            }

        }
        else
            glutSetWindowTitle("Nothing was clicked on!");


        // Restore the projection matrix
        glMatrixMode(GL_PROJECTION);
    }
    glPopMatrix();

    // Go back to modelview for normal rendering
    glMatrixMode(GL_MODELVIEW);

    glutPostRedisplay();
}

void mouse( int button, int state, int x, int y)
{
    tbMouse(button, state, x, y);

    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        processSelection(x, y);

    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        pickedObj = -1;
        glutPostRedisplay();
    }
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

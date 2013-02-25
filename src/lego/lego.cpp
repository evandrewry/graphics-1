#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif
#include <math.h>

using namespace std;

#define LEGO_HEIGHT 10
#define LEGO_WIDTH 20
#define WALL_WIDTH 2
#define LEGO_LENGTH 30
#define PEG_HEIGHT 3
#define PEG_RADIUS 3
#define NUM_PEG_VERTICES 30
#define QUAD_CYCLE(verts, n) \
{ \
    int i; \
    glBegin(GL_QUAD_STRIP); { \
        for (i = 0; i < n; i++) glVertex3fv(&verts[i][0]); \
        glVertex3fv(&verts[0][0]); \
        glVertex3fv(&verts[1][0]); \
    } glEnd(); \
}

GLuint pegDL, blockDL, index;

//LEGO VERTICES

//main eight
static GLfloat vertex_a[3] = {
    -LEGO_LENGTH/2,
    -LEGO_WIDTH/2,
    -LEGO_HEIGHT/2
};
static GLfloat vertex_b[3] = {
    -LEGO_LENGTH/2,
    -LEGO_WIDTH/2,
    LEGO_HEIGHT/2
};
static GLfloat vertex_c[3] = {
    LEGO_LENGTH/2,
    -LEGO_WIDTH/2,
    -LEGO_HEIGHT/2
};
static GLfloat vertex_d[3] = {
    LEGO_LENGTH/2,
    -LEGO_WIDTH/2,
    LEGO_HEIGHT/2
};
static GLfloat vertex_e[3] = {
    LEGO_LENGTH/2,
    LEGO_WIDTH/2,
    -LEGO_HEIGHT/2
};
static GLfloat vertex_f[3] = {
    LEGO_LENGTH/2,
    LEGO_WIDTH/2,
    LEGO_HEIGHT/2
};
static GLfloat vertex_g[3] = {
    -LEGO_LENGTH/2,
    LEGO_WIDTH/2,
    -LEGO_HEIGHT/2
};
static GLfloat vertex_h[3] = {
    -LEGO_LENGTH/2,
    LEGO_WIDTH/2,
    LEGO_HEIGHT/2
};
static GLfloat inner_vertex_a[3] = {
    -(LEGO_LENGTH/2 - WALL_WIDTH),
    -(LEGO_WIDTH/2 - WALL_WIDTH),
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_ag[3] = {
    -LEGO_LENGTH/2,
    -(LEGO_WIDTH/2 - WALL_WIDTH),
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_ac[3] = {
    -(LEGO_LENGTH/2 - WALL_WIDTH),
    -LEGO_WIDTH/2,
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_c[3] =  {
    LEGO_LENGTH/2 - WALL_WIDTH,
    -(LEGO_WIDTH/2 - WALL_WIDTH),
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_ca[3] = {
    LEGO_LENGTH/2 - WALL_WIDTH,
    -LEGO_WIDTH/2,
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_ce[3] = {
    LEGO_LENGTH/2,
    -(LEGO_WIDTH/2 - WALL_WIDTH),
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_e[3] = {
    LEGO_LENGTH/2 - WALL_WIDTH,
    LEGO_WIDTH/2 - WALL_WIDTH,
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_ec[3] = {
    LEGO_LENGTH/2,
    LEGO_WIDTH/2 - WALL_WIDTH,
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_eg[3] = {
    LEGO_LENGTH/2 - WALL_WIDTH,
    LEGO_WIDTH/2,
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_g[3] =  {
    -(LEGO_LENGTH/2 - WALL_WIDTH),
    LEGO_WIDTH/2 - WALL_WIDTH,
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_ge[3] = {
    -(LEGO_LENGTH/2 - WALL_WIDTH),
    LEGO_WIDTH/2,
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_ga[3] = {
    -LEGO_LENGTH/2,
    LEGO_WIDTH/2 - WALL_WIDTH,
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_b[3] =  {
    -(LEGO_LENGTH/2 - WALL_WIDTH),
    -(LEGO_WIDTH/2 - WALL_WIDTH),
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat inner_vertex_bd[3] = {
   -(LEGO_LENGTH/2 - WALL_WIDTH),
   -LEGO_WIDTH/2,
   LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat inner_vertex_bh[3] = {
    -LEGO_LENGTH/2,
    -(LEGO_WIDTH/2 - WALL_WIDTH),
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat inner_vertex_h[3] =  {
    -(LEGO_LENGTH/2 - WALL_WIDTH),
    LEGO_WIDTH/2 - WALL_WIDTH,
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat inner_vertex_hd[3] = {
    -(LEGO_LENGTH/2 - WALL_WIDTH),
    -LEGO_WIDTH/2,
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat inner_vertex_hh[3] = {
    -LEGO_LENGTH/2,
    -(LEGO_WIDTH/2 - WALL_WIDTH),
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat inner_vertex_d[3] =  {
    LEGO_LENGTH/2 - WALL_WIDTH,
    -(LEGO_WIDTH/2 - WALL_WIDTH),
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat inner_vertex_db[3] = {
    LEGO_LENGTH/2 - WALL_WIDTH,
    -LEGO_WIDTH/2,
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat inner_vertex_df[3] = {
    LEGO_LENGTH/2,
    -(LEGO_WIDTH/2 - WALL_WIDTH),
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat inner_vertex_f[3] =  {
    LEGO_LENGTH/2 - WALL_WIDTH,
    LEGO_WIDTH/2 - WALL_WIDTH,
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat inner_vertex_fh[3] = {
    LEGO_LENGTH/2 - WALL_WIDTH,
    LEGO_WIDTH/2,
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat inner_vertex_fd[3] = {
    LEGO_LENGTH/2,
    LEGO_WIDTH/2 - WALL_WIDTH,
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat *lego_side_vertices[8] = {
    &vertex_a[0], &vertex_b[0],
    &vertex_c[0], &vertex_d[0],
    &vertex_e[0], &vertex_f[0],
    &vertex_g[0], &vertex_h[0]
};
static GLfloat *lego_base_corner_vertices[4][4] = {
    {   &inner_vertex_a[0],
        &inner_vertex_ac[0],
        &vertex_a[0],
        &inner_vertex_ag[0]
    },{ &inner_vertex_c[0],
        &inner_vertex_ce[0],
        &vertex_c[0],
        &inner_vertex_ca[0]
    },{ &inner_vertex_e[0],
        &inner_vertex_eg[0],
        &vertex_e[0],
        &inner_vertex_ec[0]
    },{ &inner_vertex_g[0],
        &inner_vertex_ga[0],
        &vertex_g[0],
        &inner_vertex_ge[0]
    }
};

static GLfloat *lego_base_edge_vertices[4][4] = {
    {   &inner_vertex_a[0],
        &inner_vertex_ac[0],
        &inner_vertex_ca[0],
        &inner_vertex_c[0]
    },{ &inner_vertex_c[0],
        &inner_vertex_ce[0],
        &inner_vertex_ec[0],
        &inner_vertex_e[0]
    },{ &inner_vertex_e[0],
        &inner_vertex_eg[0],
        &inner_vertex_ge[0],
        &inner_vertex_g[0]
    },{ &inner_vertex_g[0],
        &inner_vertex_ga[0],
        &inner_vertex_ag[0],
        &inner_vertex_a[0]
    }
};

static GLfloat *lego_inner_face_vertices[8] = {
    &inner_vertex_a[0],
    &inner_vertex_b[0],
    &inner_vertex_c[0],
    &inner_vertex_d[0],
    &inner_vertex_e[0],
    &inner_vertex_f[0],
    &inner_vertex_g[0],
    &inner_vertex_h[0]
};

static GLfloat peg_base_center[3] = {
    0,
    LEGO_WIDTH/4,
    LEGO_HEIGHT/2
};

static GLfloat peg_top_center[3] = {
    0,
    LEGO_WIDTH/4,
    LEGO_HEIGHT/2 + PEG_HEIGHT
};

static GLfloat peg_base_vertices[NUM_PEG_VERTICES][3];
static GLfloat peg_top_vertices[NUM_PEG_VERTICES][3];
static GLfloat peg_face_vertices[NUM_PEG_VERTICES * 2][3];


static void lego_sides()
{
    QUAD_CYCLE(lego_side_vertices, 8);
}

static void lego_top()
{
    glBegin(GL_QUADS); {
        glVertex3fv(vertex_b);
        glVertex3fv(vertex_d);
        glVertex3fv(vertex_f);
        glVertex3fv(vertex_h);
    } glEnd();
}

static void lego_top_inner()
{
    glBegin(GL_QUADS); {
        glVertex3fv(inner_vertex_b);
        glVertex3fv(inner_vertex_d);
        glVertex3fv(inner_vertex_f);
        glVertex3fv(inner_vertex_h);
    } glEnd();
}

static void lego_inner_faces()
{
    QUAD_CYCLE(lego_inner_face_vertices, 8);
}

static void lego_peg_faces() {
    QUAD_CYCLE(peg_face_vertices, NUM_PEG_VERTICES * 2);
}
    

static void lego_base_edges()
{
    int i, j;
    glBegin(GL_QUADS);{
        for (i = 0; i < 4; i++){
            for (j = 0; j < 4; j++){
                glVertex3fv(&lego_base_edge_vertices[i][j][0]);
            }
        }
    } glEnd();
}

static void lego_peg_top() {
    int i;
    glBegin(GL_TRIANGLE_FAN); {
        glVertex3fv(peg_top_center);
        for (i = 0; i < NUM_PEG_VERTICES; i++) {
            glVertex3fv(peg_top_vertices[i]);
        }
        glVertex3fv(peg_top_vertices[0]);
    } glEnd();

}


static void lego_base_corners()
{
    int i, j;
    for (i = 0; i < 4; i++) {
        glBegin(GL_TRIANGLE_FAN); {
            for (j = 0; j < 4; j++) {
                glVertex3fv(&lego_base_corner_vertices[i][j][0]);
            }
        } glEnd();
    }
}

static void lego_base()
{
    lego_base_corners();
    lego_base_edges();
}

static void peg()
{
    lego_peg_faces();
    lego_peg_top();
}

static void initPegs() {
    int i;
    GLfloat x, y, *bfv, *bv, *tfv, *tv;
    for (i = 0; i < NUM_PEG_VERTICES; i++) {
        x = peg_base_center[0] + PEG_RADIUS * cos(2 * M_PI * i / NUM_PEG_VERTICES);
        y = peg_base_center[1] + PEG_RADIUS * sin(2 * M_PI * i / NUM_PEG_VERTICES);

        bfv = peg_face_vertices[2 * i];
        bv = peg_base_vertices[i];
        tfv = peg_face_vertices[2 * i + 1];
        tv = peg_top_vertices[i];

        bfv[0] = bv[0] = tfv[0] = tv[0] = x; 
        bfv[1] = bv[1] = tfv[1] = bv[1] = y; 
        bfv[2] = bv[2] = LEGO_HEIGHT/2;
        tfv[2] = tv[2] = LEGO_HEIGHT/2 + PEG_HEIGHT;
    }
    
    
    pegDL = glGenLists (1);
    glNewList(pegDL, GL_COMPILE); {
        peg();
    } glEndList();
}

static void initBlock() {
    blockDL = glGenLists (1);
    glNewList(blockDL, GL_COMPILE); {
        lego_sides();
        lego_base();
        lego_inner_faces();
    } glEndList();
}

static void initLego() {
    int i;
    GLfloat x, y, *bfv, *bv, *tfv, *tv;
    for (i = 0; i < NUM_PEG_VERTICES; i++) {
        x = peg_base_center[0] + PEG_RADIUS * cos(2 * M_PI * i / NUM_PEG_VERTICES);
        y = peg_base_center[1] + PEG_RADIUS * sin(2 * M_PI * i / NUM_PEG_VERTICES);

        bfv = peg_face_vertices[2 * i];
        bv = peg_base_vertices[i];
        tfv = peg_face_vertices[2 * i + 1];
        tv = peg_top_vertices[i];

        bfv[0] = bv[0] = tfv[0] = tv[0] = x; 
        bfv[1] = bv[1] = tfv[1] = tv[1] = y; 
        bfv[2] = bv[2] = LEGO_HEIGHT/2;
        tfv[2] = tv[2] = LEGO_HEIGHT/2 + PEG_HEIGHT;
    }

    //pegDL = glGenLists(1);
    //glNewList(pegDL, GL_COMPILE); {
        //peg();
    //} glEndList();

    
    //index = glGenLists (2);
    //glNewList(index, GL_COMPILE); {
        //peg();
    //} glEndList();

    //glNewList(index + 1, GL_COMPILE); {
        //lego_sides();
        //lego_base();
        //lego_inner_faces();
    //} glEndList();
    //initBlock();
    //initPegs();
}

void lego()
{
    GLubyte lists[2];
    //glRotatef(
    initLego();
    lego_sides();
    lego_base();
    lego_inner_faces();
    lego_top();
    peg(); 
    glPushMatrix();
        glTranslatef(0, -3 * PEG_RADIUS, 0);
        peg();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(3 * PEG_RADIUS, 0, 0);
        peg();
        glTranslatef(0, -3 * PEG_RADIUS, 0);
        peg();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-3 * PEG_RADIUS, 0, 0);
        peg();
        glTranslatef(0, -3 * PEG_RADIUS, 0);
        peg();
    glPopMatrix();




    //lists[0] = 0;//blockDL;
    //lists[1] = 1;//pegDL;
    //glListBase(index);
    //glCallLists(2, GL_UNSIGNED_BYTE, lists);
    //
    //glCallList(pegDL);
}

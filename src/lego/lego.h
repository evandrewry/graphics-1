#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#define LEGO_HEIGHT (LEGO_WIDTH * 0.6)
#define LEGO_WIDTH 18.0
#define WALL_WIDTH (LEGO_WIDTH / 18.0)
#define LEGO_LENGTH (LEGO_WIDTH * 1.5)
#define PEG_HEIGHT (LEGO_HEIGHT / 6.0)
#define PEG_RADIUS (LEGO_WIDTH / 6.0)
#define INNER_PEG_RADIUS (LEGO_WIDTH * 0.2)
#define PEG_EDGE_TO_EDGE_DISTANCE \
    ((LEGO_LENGTH - (6.0 * PEG_RADIUS)) / 3.0)
#define PEG_CENTER_TO_CENTER_DISTANCE \
    (2 * PEG_RADIUS + PEG_EDGE_TO_EDGE_DISTANCE)
#define STRUT_WIDTH WALL_WIDTH
#define STRUT_LENGTH  \
    ((LEGO_WIDTH - 2 * WALL_WIDTH - 2 * INNER_PEG_RADIUS) / 2)
#define NUM_PEG_VERTICES 100
#define STRUT_HEIGHT (LEGO_HEIGHT * 0.75)

void lego();
void multilego(int);

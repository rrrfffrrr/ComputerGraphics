#include <gl/glut.h>
#include <stdlib.h>
#include <math.h>

/*
    === Input ===
    keyboard 1,2,3,4: Select point
    Mouse left button: Move selected point

    === Warning ===
    Cannot draw spline correctly when reshape the window after first draw
*/

int selectionPoint = 0;
GLfloat ctrlPts[4][3] = {{-40.0, -40.0, 0.0}, {-10.0, 200.0, 0.0}, {10.0, -200.0, 0.0}, {40.0,40.0, 0.0}};
int windowWidth = 420, windowHeight = 420;

void init()
{
    glClearColor(1.0, 1.0, 1.0, 0.0);

    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, *ctrlPts);
    glEnable(GL_MAP1_VERTEX_3);
}

void displayFcn()
{
    glClear(GL_COLOR_BUFFER_BIT);
    GLint k;

    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINE_STRIP);
    for(k=0; k<=50; k++)
        glEvalCoord1f(GLfloat(k)/50.0);
    glEnd();

    glColor3f(1.0, 0.0, 0.0);
    glPointSize(5.0);
    glBegin(GL_POINTS);
    for(k=0; k<4; k++)
        glVertex3fv(&ctrlPts[k][0]);
    glEnd();

    glFlush();
}

void reshapeFcn(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    float halfWidth = 0.5 * width;
    float halfHeight = 0.5 * height;
    glViewport(0, 0, width, height);
    gluOrtho2D(-halfWidth, halfWidth, -halfHeight, halfHeight);
}

void keyboardFcn(unsigned char key, int x, int y) {
    switch(key) {
        case '1': selectionPoint = 0; break;
        case '2': selectionPoint = 1; break;
        case '3': selectionPoint = 2; break;
        case '4': selectionPoint = 3; break;
    }
}

void mouseFcn(int button, int state, int x, int y) {
    if (button == 0 && state == 0) {
        ctrlPts[selectionPoint][0] = x - (0.5 * windowWidth);
        ctrlPts[selectionPoint][1] = (0.5 * windowHeight) - y;
        glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, *ctrlPts);
        glutPostRedisplay();
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowPosition(50,50);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("opengl Bezier curve");

    init();
    glutKeyboardFunc(keyboardFcn);
    glutMouseFunc(mouseFcn);
    glutDisplayFunc(displayFcn);
    glutReshapeFunc(reshapeFcn);
    glutMainLoop();

    return 0;
}

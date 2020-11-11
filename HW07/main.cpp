#include <GL/glut.h>
#include <cstdlib>
#include <cmath>
#include <iostream>

/*
    Keyboard
* Translate: WS AD QE
* Rotation: YU HJ NM
* Scale: RF
* Change mode: []
* Change axis: P

    Mouse
* Left: Plus
* Right: Minus
*/

using namespace std;

GLdouble degtorad = M_PI/180.0;
GLdouble radtodeg = 180.0/M_PI;

namespace Vec3 {
    typedef GLfloat Type [3];
    void Add(Type lh, Type rh, Type output) {
        for (int i = 0; i < 3; ++i)
            output[i] = lh[i] + rh[i];
    }
    void AddSafe(Type lh, Type rh, Type output) {
        Type temp;
        Add(lh, rh, temp);
        memcpy(&output[0], &temp[0], sizeof(output[0]) * 3);
    }
    void Console(Type value) {
        std::cout << value[0] << ", " << value[1] << ", " << value[2] << "\n";
    }
}
namespace Matrix4x4 {
    typedef GLfloat Type [4][4];

    void SetIdentity(Type output)
    {
        GLint row, col;
        for(row = 0; row < 4; row++)
            for(col = 0; col < 4; col++)
                output[row][col] = (row == col);
    }

    void Multiply(Type lh, Type rh, Type output) {
        GLint row, col;
        for(row = 0; row < 4; row++)
            for(col = 0; col < 4; col++)
                output[row][col] = lh[row][0] * rh[0][col] + lh[row][1] * rh[1][col] + lh[row][2] * rh[2][col] + lh[row][3] * rh[3][col];
    }
    void MultiplySafe(Type lh, Type rh, Type output) {
        Type temp;
        Multiply(lh, rh, temp);
        memcpy(&output[0][0], &temp[0][0], sizeof(output[0][0]) * 16);
    }

    void Multiply(Type lh, Vec3::Type rh, Vec3::Type output) {
        for(GLint i = 0; i < 3; i++)
            output[i] = lh[i][0] * rh[0] + lh[i][1] * rh[1] + lh[i][2] * rh[2] + lh[i][3];
    }
    void MultiplySafe(Type lh, Vec3::Type rh, Vec3::Type output) {
        Vec3::Type temp;
        Multiply(lh, rh, temp);
        memcpy(&output[0], &temp[0], sizeof(output[0]) * 3);
    }
    void Console(Type value) {
        for(int i = 0; i < 4; ++i)
            std::cout << value[i][0] << ", " << value[i][1] << ", " << value[i][2] << ", " << value[i][3] << "\n";
    }
}
namespace Utils {
    namespace Primitives {
        typedef GLfloat Box[8][3];
        typedef GLfloat BoxColor[6][3];

        namespace Prototype {
            const Box Box = {
                { 25, -25,  25}, {-25, -25,  25},
                {-25, -25, -25}, { 25, -25, -25},
                { 25,  25,  25}, {-25,  25,  25},
                {-25,  25, -25}, { 25,  25, -25}
            };
            const BoxColor BoxColor = {
                {1, 0, 0}, {1, 1, 0}, {0, 1, 0},
                {0, 0, 1}, {1, 1, 1}, {1, 0, 1}
            };
        }

        void SetBox(Box vertices, BoxColor color) {
            memcpy(&vertices[0][0], &Prototype::Box[0][0], 8 * 3 * sizeof(vertices[0][0]));
            memcpy(&color[0][0], &Prototype::BoxColor[0][0], 6 * 3 * sizeof(color[0][0]));
        }
    }

    void DisplayQuad(GLfloat a[], GLfloat b[], GLfloat c[], GLfloat d[], GLfloat color[])
    {
        glColor3fv(color);
        glBegin(GL_POLYGON);
            glVertex3fv(a);
            glVertex3fv(b);
            glVertex3fv(c);
            glVertex3fv(d);
        glEnd();
    }
    void DisplayCube(Primitives::Box vertices, Primitives::BoxColor colors) {
        DisplayQuad(vertices[0], vertices[1], vertices[2], vertices[3], colors[0]);
        DisplayQuad(vertices[1], vertices[5], vertices[4], vertices[0], colors[1]);
        DisplayQuad(vertices[2], vertices[1], vertices[5], vertices[6], colors[2]);
        DisplayQuad(vertices[3], vertices[2], vertices[6], vertices[7], colors[3]);
        DisplayQuad(vertices[4], vertices[0], vertices[3], vertices[7], colors[4]);
        DisplayQuad(vertices[5], vertices[6], vertices[7], vertices[4], colors[5]);
    }
    void DisplayAxis(GLfloat size = 200) {
        glBegin(GL_LINES);
            glColor3f(1, 0, 0);
            glVertex3f(-size, 0, 0);
            glVertex3f( size, 0, 0);

            glColor3f(0, 1, 0);
            glVertex3f(0,  size, 0);
            glVertex3f(0, -size, 0);

            glColor3f(0, 0, 1);
            glVertex3f(0, 0,  size);
            glVertex3f(0, 0, -size);
        glEnd();
    }
}

class Transform {
private:
    Matrix4x4::Type CompositeMatrix;

public:
    Transform() {
        Matrix4x4::SetIdentity(CompositeMatrix);
    }

public:
    void SetIdentity() {
        Matrix4x4::SetIdentity(CompositeMatrix);
    }
    void Translate(GLfloat tx, GLfloat ty, GLfloat tz) {
        Matrix4x4::Type temp;
        Matrix4x4::SetIdentity(temp);

        temp[0][3] = tx;
        temp[1][3] = ty;
        temp[2][3] = tz;

        Matrix4x4::MultiplySafe(temp, CompositeMatrix, CompositeMatrix);
    }
    void Scale(GLfloat sx, GLfloat sy, GLfloat sz) {
        Matrix4x4::Type temp;
        Matrix4x4::SetIdentity(temp);

        temp[0][0] = sx;
        temp[1][1] = sy;
        temp[2][2] = sz;

        Matrix4x4::MultiplySafe(temp, CompositeMatrix, CompositeMatrix);
    }
    void RotateX(GLfloat radian) {
        Matrix4x4::Type temp;
        Matrix4x4::SetIdentity(temp);

        temp[1][1] = cos(radian);
        temp[1][2] = -sin(radian);
        temp[2][1] = sin(radian);
        temp[2][2] = cos(radian);

        Matrix4x4::MultiplySafe(temp, CompositeMatrix, CompositeMatrix);
    }
    void RotateXDegree(GLfloat degree) { RotateX(degree * degtorad); }
    void RotateY(GLfloat radian) {
        Matrix4x4::Type temp;
        Matrix4x4::SetIdentity(temp);

        temp[0][0] = cos(radian);
        temp[0][2] = sin(radian);
        temp[2][0] = -sin(radian);
        temp[2][2] = cos(radian);

        Matrix4x4::MultiplySafe(temp, CompositeMatrix, CompositeMatrix);
    }
    void RotateYDegree(GLfloat degree) { RotateY(degree * degtorad); }
    void RotateZ(GLfloat radian) {
        Matrix4x4::Type temp;
        Matrix4x4::SetIdentity(temp);

        temp[0][0] = cos(radian);
        temp[0][1] = -sin(radian);
        temp[1][0] = sin(radian);
        temp[1][1] = cos(radian);

        Matrix4x4::MultiplySafe(temp, CompositeMatrix, CompositeMatrix);
    }
    void RotateZDegree(GLfloat degree) { RotateZ(degree * degtorad); }
    void TransformPoint(Vec3::Type input, Vec3::Type output) {
        Matrix4x4::MultiplySafe(CompositeMatrix, input, output);
    }
};
class CEntity {
public:
    virtual void Display() {};
};
class CBox : public CEntity {
private:
    Vec3::Type Origin;
    Utils::Primitives::Box Vertices;
    Utils::Primitives::BoxColor Colors;

public:
    Transform Transform;

public:
    CBox() { Utils::Primitives::SetBox(Vertices, Colors); }

public:
    void Translate(GLfloat tx, GLfloat ty, GLfloat tz) {
        Transform.Translate(tx, ty, tz);
    }
    void Scale(GLfloat sx, GLfloat sy, GLfloat sz) {
        Transform.Translate(-Origin[0], -Origin[1], -Origin[2]);
        Transform.Scale(sx, sy, sz);
        Transform.Translate(Origin[0], Origin[1], Origin[2]);
    }
    void RotateX(GLfloat degree) {
        Transform.Translate(-Origin[0], -Origin[1], -Origin[2]);
        Transform.RotateXDegree(degree);
        Transform.Translate(Origin[0], Origin[1], Origin[2]);
    }
    void RotateY(GLfloat degree) {
        Transform.Translate(-Origin[0], -Origin[1], -Origin[2]);
        Transform.RotateYDegree(degree);
        Transform.Translate(Origin[0], Origin[1], Origin[2]);
    }
    void RotateZ(GLfloat degree) {
        Transform.Translate(-Origin[0], -Origin[1], -Origin[2]);
        Transform.RotateZDegree(degree);
        Transform.Translate(Origin[0], Origin[1], Origin[2]);
    }
    void ApplyTransform() {
        for(GLint i = 0; i < 8; ++i) {
            Transform.TransformPoint(Vertices[i], Vertices[i]);
        }
        Transform.TransformPoint(Origin, Origin);
        Transform.SetIdentity();
    }
    void Display() {
        Utils::DisplayCube(Vertices, Colors);
    };

    void DebugOrigin() {
        Vec3::Console(Origin);
        std::cout <<"\n";
    }

    void DebugVertices() {
        for(int i = 0; i < 8; ++i)
            Vec3::Console(Vertices[i]);
        std::cout <<"\n";
    }

    void DebugColor() {
        for(int i = 0; i < 6; ++i)
            Vec3::Console(Colors[i]);
        std::cout <<"\n";
    }
};

enum TransformMode {
    TRANS_TRANSLATION = 0,
    TRANS_ROTATION = 1,
    TRANS_SCALE = 2,
};

enum Axis {
    AXIS_X = 0,
    AXIS_Y = 1,
    AXIS_Z = 2,
};

GLfloat rotation = 15;
GLfloat speed = 10;
GLfloat scaleMul = 1.25;
GLfloat scaleDiv = 1.0/scaleMul;
GLint mode = 0;
GLint axis = 0;
CBox box;

void ConsoleAxisInfo() {
    std::cout << "Current axis: ";
    switch(axis) {
        case AXIS_X:
            std::cout << "X\n";
            break;
        case AXIS_Y:
            std::cout << "Y\n";
            break;
        case AXIS_Z:
            std::cout << "Z\n";
            break;
    }
}
void ConsoleInformation() {
    std::cout << "Current mode: ";
    switch(mode) {
        case TRANS_TRANSLATION:
            std::cout << "Translation\n";
            ConsoleAxisInfo();
            break;
        case TRANS_ROTATION:
            std::cout << "Rotation\n";
            ConsoleAxisInfo();
            break;
        case TRANS_SCALE:
            std::cout << "Scale\n";
            break;
    }
}

void KeyboardFunc(unsigned char key, int x, int y)
{
    std::cout << "== Keyboard" << "\n";
    switch(key) {
        /// translate
        case 'w': box.Translate( speed, 0, 0); break;
        case 's': box.Translate(-speed, 0, 0); break;
        case 'a': box.Translate(0, 0, speed); break;
        case 'd': box.Translate(0, 0, -speed); break;
        case 'q': box.Translate(0, speed, 0); break;
        case 'e': box.Translate(0,-speed, 0); break;

        /// rotate
        case 'y': box.RotateX( rotation); break;
        case 'u': box.RotateX(-rotation); break;
        case 'h': box.RotateY( rotation); break;
        case 'j': box.RotateY(-rotation); break;
        case 'n': box.RotateZ( rotation); break;
        case 'm': box.RotateZ(-rotation); break;

        /// scale
        case 'r': box.Scale(scaleMul, scaleMul, scaleMul); break;
        case 'f': box.Scale(scaleDiv, scaleDiv, scaleDiv); break;

        /// change mode
        case '[': mode = (mode == 0) ? 2 : (mode-1); ConsoleInformation(); break;
        case ']': mode = (mode + 1) % 3; ConsoleInformation(); break;

        /// change axis
        case 'p': axis = (axis + 1) % 3; ConsoleInformation(); break;
    }
    box.ApplyTransform();
    glutPostRedisplay();
}

void SpecialKeyboardFunc(int key, int x, int y) {
    std::cout << "== Special keyboard" << "\n";
    glutPostRedisplay();
}
void MouseFunction(int btn, int state, int x, int y)
{
    std::cout << "== Mouse" << "\n";
    switch(state) {
    case GLUT_DOWN:
        GLdouble dire = 0;
        switch(btn) {
            case GLUT_LEFT_BUTTON:
                dire = 1;
                break;
            case GLUT_RIGHT_BUTTON:
                dire = -1;
                break;
        }

        switch(mode) {
            case TRANS_TRANSLATION:
                switch(axis) {
                    case AXIS_X: box.Translate(dire * speed, 0, 0); break;
                    case AXIS_Y: box.Translate(0, dire * speed, 0); break;
                    case AXIS_Z: box.Translate(0, 0, dire * speed); break;
                }
                break;
            case TRANS_ROTATION:
                switch(axis) {
                    case AXIS_X: box.RotateX(dire * rotation); break;
                    case AXIS_Y: box.RotateY(dire * rotation); break;
                    case AXIS_Z: box.RotateZ(dire * rotation); break;
                }
                break;
            case TRANS_SCALE:
                if (dire > 0) dire = scaleMul;
                else if (dire < 0) dire = scaleDiv;
                else dire = 1;
                box.Scale(dire, dire, dire);
                break;
        }
        box.ApplyTransform();
        glutPostRedisplay();
        break;
    }
}

void Init()
{
    glClearColor(0.0,0.0,0.0,0.0);
    glColor3f(1.0,1.0,0.0);
    glEnable(GL_DEPTH_TEST);

    box.Transform.Translate(20, -18, 25);
    box.ApplyTransform();
}

void DisplayFunc()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(50.0, 40.0, 50.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
    Utils::DisplayAxis();

    box.Display();

    glFlush();
    glutSwapBuffers();
}

void ReshapeFunc(int w, int h)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-300, 300, -300, 300, -300, 300);
}

GLsizei winWidth=640, winHeight=480;
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("cube");

    Init();

    glutDisplayFunc(DisplayFunc);
    glutIdleFunc(DisplayFunc);

    glutReshapeFunc(ReshapeFunc);
    glutKeyboardFunc(KeyboardFunc);
    glutSpecialFunc(SpecialKeyboardFunc);
    glutMouseFunc(MouseFunction);

    glutMainLoop();
    return 0;
}

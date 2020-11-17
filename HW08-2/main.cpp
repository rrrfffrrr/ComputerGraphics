#include <GL/glut.h>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <queue>

/*
    Keyboard
* Rotate: AD
* Toggle auto rotation: S

    Mouse
* Left: Rotate left
* Right: Rotate right
*/

using namespace std;

const GLdouble degtorad = M_PI/180.0;
const GLdouble radtodeg = 180.0/M_PI;

namespace Vec3 {
    typedef GLfloat Type [3];
    void Add(const Type lh, const Type rh, Type output) {
        for (int i = 0; i < 3; ++i)
            output[i] = lh[i] + rh[i];
    }
    void AddSafe(const Type lh, const Type rh, Type output) {
        Type temp;
        Add(lh, rh, temp);
        memcpy(&output[0], &temp[0], sizeof(output[0]) * 3);
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

    void Multiply(const Type lh, const Type rh, Type output) {
        GLint row, col;
        for(row = 0; row < 4; row++)
            for(col = 0; col < 4; col++)
                output[row][col] = lh[row][0] * rh[0][col] + lh[row][1] * rh[1][col] + lh[row][2] * rh[2][col] + lh[row][3] * rh[3][col];
    }
    void MultiplySafe(const Type lh, const Type rh, Type output) {
        Type temp;
        Multiply(lh, rh, temp);
        memcpy(&output[0][0], &temp[0][0], sizeof(output[0][0]) * 16);
    }

    void Multiply(const Type lh, const Vec3::Type rh, Vec3::Type output) {
        for(GLint i = 0; i < 3; i++)
            output[i] = lh[i][0] * rh[0] + lh[i][1] * rh[1] + lh[i][2] * rh[2] + lh[i][3];
    }
    void MultiplySafe(const Type lh, const Vec3::Type rh, Vec3::Type output) {
        Vec3::Type temp;
        Multiply(lh, rh, temp);
        memcpy(&output[0], &temp[0], sizeof(output[0]) * 3);
    }
}
namespace Utils {
    namespace Primitives {
        namespace Box {
            typedef GLfloat Type[8][3];
            typedef GLfloat ColorType[6][3];

            namespace Prototype {
                const Type Box = {
                    { 25, -25,  25}, {-25, -25,  25},
                    {-25, -25, -25}, { 25, -25, -25},
                    { 25,  25,  25}, {-25,  25,  25},
                    {-25,  25, -25}, { 25,  25, -25}
                };
                const ColorType BoxColor = {
                    {1, 0, 0}, {1, 1, 0}, {0, 1, 0},
                    {0, 0, 1}, {1, 1, 1}, {1, 0, 1}
                };
            }
        }
    }

    void DisplayQuad(const GLfloat a[], const GLfloat b[], const GLfloat c[], const GLfloat d[], const GLfloat color[])
    {
        glColor3fv(color);
        glBegin(GL_POLYGON);
            glVertex3fv(a);
            glVertex3fv(b);
            glVertex3fv(c);
            glVertex3fv(d);
        glEnd();
    }
    void DisplayCube(const Primitives::Box::Type vertices, const Primitives::Box::ColorType colors) {
        DisplayQuad(vertices[0], vertices[1], vertices[2], vertices[3], colors[0]);
        DisplayQuad(vertices[1], vertices[5], vertices[4], vertices[0], colors[1]);
        DisplayQuad(vertices[2], vertices[1], vertices[5], vertices[6], colors[2]);
        DisplayQuad(vertices[3], vertices[2], vertices[6], vertices[7], colors[3]);
        DisplayQuad(vertices[4], vertices[0], vertices[3], vertices[7], colors[4]);
        DisplayQuad(vertices[5], vertices[6], vertices[7], vertices[4], colors[5]);
    }
    void DisplayAxis(const GLfloat size = 200) {
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
public:
    Matrix4x4::Type CompositeMatrix;

public:
    Transform() {
        Matrix4x4::SetIdentity(CompositeMatrix);
    }
public:
    void GetPosition(Vec3::Type output) {
        for(int i = 0; i < 3; ++i)
            output[i] = CompositeMatrix[i][3];
    }
    void GetForward(Vec3::Type output) {
        for(int i = 0; i < 3; ++i)
            output[i] = CompositeMatrix[i][0];
    }
    void GetUp(Vec3::Type output) {
        for(int i = 0; i < 3; ++i)
            output[i] = CompositeMatrix[i][1];
    }
    void GetRight(Vec3::Type output) {
        for(int i = 0; i < 3; ++i)
            output[i] = CompositeMatrix[i][2];
    }

public:
    void SetIdentity() {
        Matrix4x4::SetIdentity(CompositeMatrix);
    }
    void Translate(const GLfloat tx, const GLfloat ty, const GLfloat tz) {
        Matrix4x4::Type temp;
        Matrix4x4::SetIdentity(temp);

        temp[0][3] = tx;
        temp[1][3] = ty;
        temp[2][3] = tz;

        Matrix4x4::MultiplySafe(temp, CompositeMatrix, CompositeMatrix);
    }
    void Scale(const GLfloat sx, const GLfloat sy, const GLfloat sz) {
        Matrix4x4::Type temp;
        Matrix4x4::SetIdentity(temp);

        temp[0][0] = sx;
        temp[1][1] = sy;
        temp[2][2] = sz;

        Matrix4x4::MultiplySafe(temp, CompositeMatrix, CompositeMatrix);
    }
    void RotateX(const GLfloat radian) {
        Matrix4x4::Type temp;
        Matrix4x4::SetIdentity(temp);

        temp[1][1] = cos(radian);
        temp[1][2] = -sin(radian);
        temp[2][1] = sin(radian);
        temp[2][2] = cos(radian);

        Matrix4x4::MultiplySafe(temp, CompositeMatrix, CompositeMatrix);
    }
    void RotateXDegree(const GLfloat degree) { RotateX(degree * degtorad); }
    void RotateY(const GLfloat radian) {
        Matrix4x4::Type temp;
        Matrix4x4::SetIdentity(temp);

        temp[0][0] = cos(radian);
        temp[0][2] = sin(radian);
        temp[2][0] = -sin(radian);
        temp[2][2] = cos(radian);

        Matrix4x4::MultiplySafe(temp, CompositeMatrix, CompositeMatrix);
    }
    void RotateYDegree(const GLfloat degree) { RotateY(degree * degtorad); }
    void RotateZ(const GLfloat radian) {
        Matrix4x4::Type temp;
        Matrix4x4::SetIdentity(temp);

        temp[0][0] = cos(radian);
        temp[0][1] = -sin(radian);
        temp[1][0] = sin(radian);
        temp[1][1] = cos(radian);

        Matrix4x4::MultiplySafe(temp, CompositeMatrix, CompositeMatrix);
    }
    void RotateZDegree(const GLfloat degree) { RotateZ(degree * degtorad); }
    void TransformPoint(const Vec3::Type input, Vec3::Type output) {
        Matrix4x4::MultiplySafe(CompositeMatrix, input, output);
    }
};

class CEntity {
public: /// variable
    Transform Transform;

public: /// Transform
    void Translate(GLfloat tx, GLfloat ty, GLfloat tz) {
        Transform.Translate(tx, ty, tz);
    }
    void Scale(GLfloat sx, GLfloat sy, GLfloat sz) {
        Vec3::Type origin; Transform.GetPosition(origin);
        Scale(sx, sy, sz, origin);
    }
    void Scale(GLfloat sx, GLfloat sy, GLfloat sz, Vec3::Type origin) {
        Transform.Translate(-origin[0], -origin[1], -origin[2]);
        Transform.Scale(sx, sy, sz);
        Transform.Translate(origin[0], origin[1], origin[2]);
    }
    void RotateX(GLfloat degree) {
        Vec3::Type origin; Transform.GetPosition(origin);
        RotateX(degree, origin);
    }
    void RotateX(GLfloat degree, Vec3::Type origin) {
        Transform.Translate(-origin[0], -origin[1], -origin[2]);
        Transform.RotateXDegree(degree);
        Transform.Translate(origin[0], origin[1], origin[2]);
    }
    void RotateY(GLfloat degree) {
        Vec3::Type origin; Transform.GetPosition(origin);
        RotateY(degree, origin);
    }
    void RotateY(GLfloat degree, Vec3::Type origin) {
        Transform.Translate(-origin[0], -origin[1], -origin[2]);
        Transform.RotateYDegree(degree);
        Transform.Translate(origin[0], origin[1], origin[2]);
    }
    void RotateZ(GLfloat degree) {
        Vec3::Type origin; Transform.GetPosition(origin);
        RotateZ(degree, origin);
    }
    void RotateZ(GLfloat degree, Vec3::Type origin) {
        Transform.Translate(-origin[0], -origin[1], -origin[2]);
        Transform.RotateZDegree(degree);
        Transform.Translate(origin[0], origin[1], origin[2]);
    }
public: /// scene methods
    virtual void Update(double delta_time) {};
    virtual void Display() {};
};

class CBox : public CEntity {
private: /// variables
    float elapse;

public: /// Inheritance
    void Update(double delta_time) {
        elapse += delta_time * degtorad * 180;
        Transform.CompositeMatrix[1][3] = cos(elapse) * 40;
    }
    void Display() {
        Utils::Primitives::Box::Type vertices;
        for(GLint i = 0; i < 8; ++i) {
            Transform.TransformPoint(Utils::Primitives::Box::Prototype::Box[i], vertices[i]);
        }
        Utils::DisplayCube(vertices, Utils::Primitives::Box::Prototype::BoxColor);
    };
};
class CCamera : public CEntity {
public: /// variables
    float fov;
    float aspect;
    float zNear;
    float zFar;

    bool isRotating;
    CEntity* Target;

public: /// Ctor/Dtor
    CCamera(float fov = 90, float aspect = 1, float _near = 0.3, float _far = 1000) : CEntity(), fov(fov), aspect(aspect), zNear(_near), zFar(_far), isRotating(false) {}

public: /// methods
    void OnReshapeTarget(int width, int height) {
        aspect = static_cast<float>(width)/height;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(fov, aspect, zNear, zFar);
    }

public: /// Inheritance
    void Update(double dt) {
        if (isRotating) {
            Transform.RotateYDegree(dt * 90);
        }
    }
    void Display() {
        Vec3::Type position, forwardVector;
        Transform.GetPosition(position); Transform.GetForward(forwardVector);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        if (Target != NULL) {
            gluLookAt(position[0], position[1], position[2], Target->Transform.CompositeMatrix[0][3], Target->Transform.CompositeMatrix[1][3], Target->Transform.CompositeMatrix[2][3], 0.0, 1.0, 0.0);
        } else
            gluLookAt(position[0], position[1], position[2], position[0] + forwardVector[0], position[1] + forwardVector[1], position[2] + forwardVector[2], 0.0, 1.0, 0.0);
    }
};


/// Initialize program data
CCamera cam;
CBox box;
int old_tick;

void Init() {
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);

    cam.Translate(-300, 45, 0);
    cam.Target = &box;

    old_tick = glutGet(GLUT_ELAPSED_TIME);
}

/// Callbacks
void KeyboardFunc(unsigned char key, int x, int y) {
    std::cout << "== Keyboard" << "\n";
    switch(key) {
        case 'a': cam.Transform.RotateYDegree(-10); break;
        case 'd': cam.Transform.RotateYDegree(10); break;
        case 's': cam.isRotating = !cam.isRotating; break;
    }
    glutPostRedisplay();
}

void SpecialKeyboardFunc(int key, int x, int y) {
    std::cout << "== Special keyboard" << "\n";
}
void MouseFunction(int btn, int state, int x, int y) {
    std::cout << "== Mouse" << "\n";
    switch(state) {
    case GLUT_DOWN:
        switch(btn) {
            case GLUT_LEFT_BUTTON:
                cam.Transform.RotateYDegree(-10);
                break;
            case GLUT_RIGHT_BUTTON:
                cam.Transform.RotateYDegree(10);
                break;
        }
        break;
    }
}

void DisplayFunc() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    cam.Display();

    Utils::DisplayAxis();
    box.Display();

    glFlush();
    glutSwapBuffers();
}

void ReshapeFunc(int width, int height) {
    glViewport(0, 0, width, height);
    cam.OnReshapeTarget(width, height);
}

void IdleFunc() {
    int tick = glutGet(GLUT_ELAPSED_TIME);
    double delta_time = 0.001 * (tick - old_tick);
    old_tick = tick;

    cam.Update(delta_time);
    box.Update(delta_time);

    glutPostRedisplay();
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
    glutIdleFunc(IdleFunc);

    glutReshapeFunc(ReshapeFunc);
    glutKeyboardFunc(KeyboardFunc);
    glutSpecialFunc(SpecialKeyboardFunc);
    glutMouseFunc(MouseFunction);

    glutMainLoop();
    return 0;
}

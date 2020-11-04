#include <GL/glut.h>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <cfloat>
#include <iostream>

/// Array of 3 elements to represent homogeneous coordinate
struct Vec3 {
    union {
        struct { float x, y, z; };
        struct { float u, v, w; };
        struct { float r, g, b; };
        float Value[3];
    };

    Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
};

/// 3x3 Matrix and it's operations
struct Matrix3x3 {
    union {
        struct { float V00, V01, V02, V10, V11, V12, V20, V21, V22; };
        float V33[3][3];
        float Value[9];
    };

    Matrix3x3(float v00 = 0, float v01 = 0, float v02 = 0, float v10 = 0, float v11 = 0, float v12 = 0, float v20 = 0, float v21 = 0, float v22 = 0) : V00(v00), V01(v01), V02(v02), V10(v10), V11(v11), V12(v12), V20(v20), V21(v21), V22(v22) {}

    /// Build identity matrix
    static void BuildIdentity(Matrix3x3* output) {
        output->V01 = output->V02 = output->V10 = output->V12 = output->V20 = output->V21 = 0;
        output->V00 = output->V11 = output->V22 = 1;
    }
    static Matrix3x3* BuildIdentity() {
        Matrix3x3* rot = new Matrix3x3();
        BuildIdentity(rot);
        return rot;
    }

    /// Build translation matrix
    static void BuildTranslationMatrix(float tx, float ty, Matrix3x3* output) {
        *output = Matrix3x3( 1, 0,  tx,
                             0, 1,  ty,
                             0, 0,  1);
    }
    static Matrix3x3* BuildTranslationMatrix(float tx, float ty) {
        Matrix3x3* rot = new Matrix3x3();
        BuildTranslationMatrix(tx, ty, rot);
        return rot;
    }

    /// Build rotation matrix
    static void BuildRotationMatrix(float degree, Matrix3x3* output) {
        float radian = M_PI * degree / 180;
        float c = cos(radian);
        float s = sin(radian);
        *output = Matrix3x3( c, -s, 0,
                             s, c,  0,
                             0, 0,  1);
    }
    static Matrix3x3* BuildRotationMatrix(float degree) {
        Matrix3x3* rot = new Matrix3x3();
        BuildRotationMatrix(degree, rot);
        return rot;
    }

    /// Build scale matrix
    static void BuildScaleMatrix(float sx, float sy, Matrix3x3* output) {
        *output = Matrix3x3( sx,    0,      0,
                             0,     sy,     0,
                             0,     0,      1);
    }
    static Matrix3x3* BuildScaleMatrix(float sx, float sy) {
        Matrix3x3* rot = new Matrix3x3();
        BuildScaleMatrix(sx, sy, rot);
        return rot;
    }

    /// Multiply with matrix or vec3
    static void Multiply(Matrix3x3* m1, Matrix3x3* m2, Matrix3x3* output) {
        for(int i = 0; i < 3; ++i) {
            for(int j = 0; j < 3; ++j) {
                output->V33[i][j] = m1->V33[i][0] * m2->V33[0][j] + m1->V33[i][1] * m2->V33[1][j] + m1->V33[i][2] * m2->V33[2][j];
            }
        }
    }
    static void Multiply(Vec3* v1, Matrix3x3* m1, Vec3* output) {
        for(int i = 0; i < 3; ++i) {
            output->Value[i] = v1->Value[0] * m1->V33[i][0] + v1->Value[1] * m1->V33[i][1] + v1->Value[2] * m1->V33[i][2];
        }
    }

    /// Multiply with matrix or vec3 and safe when parameter is same with output
    static void MultiplySafe(Matrix3x3* m1, Matrix3x3* m2, Matrix3x3* output) {
        Matrix3x3 temporal;
        Multiply(m1, m2, &temporal);
        memcpy(output->Value, temporal.Value, 9 * sizeof(float));
    }
    static void MultiplySafe(Vec3* v1, Matrix3x3* m1, Vec3* output) {
        Vec3 temporal;
        Multiply(v1, m1, &temporal);
        memcpy(output->Value, temporal.Value, 3 * sizeof(float));
    }
};

// debug only
void LogMatrix(Matrix3x3* mat) {
    #ifdef DEBUG
    std::cout << "Matrix3x3: ";
    for(int i = 0; i < 9; ++i) {
        std::cout << mat->Value[i] << ", ";
    }
    std::cout << "\n";
    #endif
}

/// 2D Transform class to represent Affine matrix
class Transform2D {
private:
    std::vector<Matrix3x3> TopStack;

public:
    /// Push and pop matrix
    void Push(Matrix3x3* m1) {
        Matrix3x3 new_top;
        Matrix3x3 top; GetTop(&top);
        Matrix3x3::Multiply(m1, &top, &new_top);

        // debug only
        #ifdef DEBUG
        std::cout << "DEBUG| Logging push" << "\n";
        std::cout << "input "; LogMatrix(m1);
        std::cout << "top "; LogMatrix(&top);
        std::cout << "new top "; LogMatrix(&new_top);
        #endif

        TopStack.push_back(new_top);
    }
    void Pop() {
        TopStack.pop_back();
    }

    /// return top to out parameter
    void GetTop(Matrix3x3* output) {
        if (TopStack.empty()) {
            Matrix3x3::BuildIdentity(output);
            return;
        }
        *output = TopStack.back();
    }

    /// transform a point
    void TransformPoint(Vec3* point, Vec3* output) {
        Matrix3x3 top; GetTop(&top);
        Matrix3x3::Multiply(point, &top, output);
    }
    Vec3* TransformPoint(Vec3* point) {
        Vec3* output = new Vec3();
        TransformPoint(point, output);
        return output;
    }

    /// transform a vector
    void TransformVector(Vec3* point, Vec3* output) {
        Matrix3x3 Matrix; GetTop(&Matrix);
        float tx = Matrix.V02; Matrix.V02 = 0;
        float ty = Matrix.V12; Matrix.V12 = 0;
        Matrix3x3::Multiply(point, &Matrix, output);
        Matrix.V02 = tx; Matrix.V12 = ty;
    }
    Vec3* TransformVector(Vec3* point) {
        Vec3* output = new Vec3();
        TransformVector(point, output);
        return output;
    }
};

/// entity that represent bullet
class CEntity {
public:
    static const float HALF_HEIGHT = 8;
    static const float HALF_WIDTH = 4;

public:
    Vec3 Color;
    Vec3 Points[3];
    Transform2D Transform;

public:
    CEntity() : Color() {
        Points[0] = Vec3(-HALF_HEIGHT, 0, 1);
        Points[1] = Vec3(HALF_HEIGHT, HALF_WIDTH, 1);
        Points[2] = Vec3(HALF_HEIGHT, -HALF_WIDTH, 1);
    };

public:
    void Display() {
        Vec3 temp;

        glColor3d(Color.r, Color.g, Color.b);
        glBegin(GL_LINE_LOOP);

        for(int i = 0; i < 3; ++i) {
            Transform.TransformPoint(&Points[i], &temp);
            glVertex2f(temp.x, temp.y);
        }

        glEnd();
    }
};

/// entity container and process render-pipe
class CScreen {
public:
    std::vector<CEntity> Entities;

public:
    void Display() {
        for(std::vector<CEntity>::iterator iter = Entities.begin(); iter != Entities.end(); ++iter) {
            iter->Display();
        }
    }
};
static CScreen screen;

/// Create entities to display
void CreateEntities(float speed, float angle, float scale_per_time, float rotation_per_time, float delta_time) {
    Matrix3x3 temporal;

    Matrix3x3 StartRotation;
    Matrix3x3::BuildRotationMatrix(90, &StartRotation); // Rotation forward to up
    Matrix3x3 StartTranslation;
    Matrix3x3::BuildTranslationMatrix(CEntity::HALF_WIDTH + 5 + speed, CEntity::HALF_HEIGHT + 1, &StartTranslation);

    CEntity entity;
    float time = 0;

    float angleToRadian = angle * M_PI / 180;

    float tx = 0, ty = 0;
    float s = 1, rot = 0;

    bool loop = true;
    while(loop) {
        loop = angleToRadian * time <= M_PI;
        tx = - speed * cos(angleToRadian * time); ty = speed * sin(angleToRadian * time) - 5 * time * time;
        s = 1 + scale_per_time * time;
        rot = rotation_per_time * time;

        entity = CEntity();
        entity.Transform.Push(&StartRotation);
        Matrix3x3::BuildScaleMatrix(s, s, &temporal);           entity.Transform.Push(&temporal);
        Matrix3x3::BuildRotationMatrix(rot, &temporal);         entity.Transform.Push(&temporal);

        Matrix3x3::BuildTranslationMatrix(tx, ty, &temporal);   entity.Transform.Push(&temporal);

        entity.Transform.Push(&StartTranslation);
        screen.Entities.push_back(entity);

        time += delta_time;
    }
}

/// main entry-point and feature
void InitFunc() {
    glClearColor(1, 1, 1, 0);
    CreateEntities(250, 270, 2, -270, 0.06);
}
void DisplayFunc() {
    glClear(GL_COLOR_BUFFER_BIT);

    screen.Display();

    glFlush();
}
void ReshapeFunc(GLint newWidth, GLint newHeight) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 640, 0, 480);
    glClear(GL_COLOR_BUFFER_BIT);
}
int main(int argc, char** argv) {
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Ballistic calculator");

    InitFunc();
    glutDisplayFunc(DisplayFunc);
    glutReshapeFunc(ReshapeFunc);

    glutMainLoop();
    return 0;
}

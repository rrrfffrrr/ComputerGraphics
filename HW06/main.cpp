#include <GL/glut.h>
#include <cstdlib>
#include <cmath>
#include <cfloat>
#include <iostream>

#include "Vec.hpp"
#include "Transform.hpp"
#include "Game.hpp"
#include "Utils.hpp"

static Game::CScene scene;

class CProjectile : public Game::CEntity {};

void drawAxis()
{
    glBegin(GL_LINES);
        glColor3f(1.0,0.0,0.0);
        glVertex3f(-200.0,0.0,0.0);
        glVertex3f(200.0,0.0,0.0);

        glColor3f(0.0,1.0,0.0);
        glVertex3f(0.0,200.0,0.0);
        glVertex3f(0.0,-200.0,0.0);

        glColor3f(0.0,0.0,1.0);
        glVertex3f(0.0,0.0,200.0);
        glVertex3f(0.0,0.0,-200.0);
    glEnd();
}

/* OLD CODE
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
}*/

void BuildTrail(float speed, float angle, float scale_per_time, float rotation_per_time, float delta_time) {
    const float MESH_SIZE = 10;
    using namespace Math::Matrix;
    Matrix4x4 temporal;
    CProjectile projectile;

    /// Initial start transform
    Matrix4x4 startRotation;
    Matrix4x4::BuildRotationYMatrix(60, &startRotation);
    Matrix4x4 startScale;
    Matrix4x4::BuildScaleMatrix(1, 0.5, 0.25, &startScale);

    /// Create mesh data
    MeshFactory factory(MESH_SIZE);
    factory.SetMesh(&projectile.Mesh, MeshFactory::MESH_BOX);

    float time = 0;

    float angleToRadian = angle * M_PI / 180;

    float tx = 0, ty = 0;
    float s = 1, rot = 0;

    while(angleToRadian * time <= M_PI) {
        tx = - speed * cos(angleToRadian * time); ty = speed * sin(angleToRadian * time) - 5 * time * time;
        s = 1 + scale_per_time * time;
        rot = rotation_per_time * time;

        Matrix4x4::BuildScaleMatrix(s, s, s, &temporal);           projectile.Transform.Push(&temporal);
        Matrix4x4::BuildRotationXMatrix(rot, &temporal);         projectile.Transform.Push(&temporal);
        Matrix4x4::BuildRotationYMatrix(-rot, &temporal);         projectile.Transform.Push(&temporal);
        Matrix4x4::BuildRotationZMatrix(rot, &temporal);         projectile.Transform.Push(&temporal);
        Matrix4x4::BuildTranslationMatrix(tx, ty, 0, &temporal);   projectile.Transform.Push(&temporal);

        projectile.Transform.Push(&startRotation);
        scene.Attach(&projectile);
        projectile.Transform.Clear();
        time += delta_time;
    }
}

/// main entry-point and feature
void InitFunc() {
    scene.Init();
    BuildTrail(200, 270, 2, -270, 0.06);
}
void IdleFunc() {
    scene.Tick();
    glutPostRedisplay();
}
void ReshapeFunc(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-300, 300, -300, 300, -300, 300);
}
void DisplayFunc() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    drawAxis();

    scene.Render();

    glFlush();
    glutSwapBuffers();
}
int main(int argc, char** argv) {
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Ballistic calculator");

    InitFunc();
    glutDisplayFunc(DisplayFunc);
    glutReshapeFunc(ReshapeFunc);
    glutIdleFunc(IdleFunc);

    glutMainLoop();
    return 0;
}

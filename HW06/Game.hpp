#ifndef __HEADER_GAME
#define __HEADER_GAME
#include <vector>

#include "Vec.hpp"
#include "Matrix.hpp"
#include "Transform.hpp"

namespace Game {
    using namespace Math::Vec;
    using namespace Math::Matrix;
    using namespace Math::Transform;

    struct Vertex {
        Vec4 Position;
        Vec3 Color;
    };

    class MeshRenderer {
    public:
        int RenderMode;

        std::vector<Vertex> Vertexs;
        std::vector<Vec3> Triangles;
    public:
        MeshRenderer() : RenderMode(GL_TRIANGLES) {}

    public:
        void Render(Transform3D* transform) {
            Vec4 temp;
            Vec3 temp_tri;
            Vertex temp_origin;

            glBegin(RenderMode);

            for(std::vector<Vec3>::iterator iter = Triangles.begin(); iter != Triangles.end(); ++iter) {
                 for(int j = 0; j < 3; ++j) {
                    temp_origin = Vertex(Vertexs[iter->Value[j]]);
                    transform->TransformPoint(&temp_origin.Position, &temp);
                    glColor3f(temp_origin.Color.r, temp_origin.Color.g, temp_origin.Color.b);
                    glVertex3f(temp.x, temp.y, temp.z);
                }
            }

            glEnd();
        }
    };

    class CEntity {
    public:
        Transform3D Transform;
        MeshRenderer Mesh;

    public:
        virtual void Create() {}
        virtual void Update() {}
        virtual void Render() {
            Mesh.Render(&Transform);
        }
    };

    class CScene {
    private:
        std::vector<CEntity> Entities;

    public:
        void Attach(CEntity* entity) {
            Entities.push_back(*entity);
            entity->Create();
        }

    public:
        void Init() {
            glClearColor(0, 0, 0, 0);
            glEnable(GL_DEPTH_TEST);
        }
        void Tick() {
            for(std::vector<CEntity>::iterator iter = Entities.begin(); iter != Entities.end(); ++iter) {
                iter->Update();
            }
        }
        void Render() {
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(100, 50, 100, 0, 0, 0, 0, 1, 0);

            for(std::vector<CEntity>::iterator iter = Entities.begin(); iter != Entities.end(); ++iter) {
                iter->Render();
            }
        }
    };
}

#endif // __HEADER_GAME

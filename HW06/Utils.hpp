#ifndef __HEADER_UTILS
#define __HEADER_UTILS
#include "Game.hpp"
using namespace Game;

class MeshFactory {
public:
    enum MeshType {
        MESH_BOX,
        MESH_ICOSPHERE,
    };

public:
    float Size;

public:
    MeshFactory(float size = 1) : Size(size) {}

public:
    void SetMesh(MeshRenderer* renderer, MeshType type) {
        renderer->Vertexs.clear();
        renderer->Triangles.clear();

        switch(type) {
        case MESH_BOX: SetBox(renderer); break;
        case MESH_ICOSPHERE: SetICOSphere(renderer); break;
        default: break;
        }
    }

private:
    // Helper method
    inline void SetPosition(MeshRenderer* renderer, Vertex* vert, float x, float y, float z) {
        vert->Position = Vec4(x, y, z, 1);
        renderer->Vertexs.push_back(*vert);
    }

    void SetBox(MeshRenderer* renderer) {
        /// vertexs
        Vertex vert;

        vert.Color = Math::Vec::Vec3(1, 0, 0);
        SetPosition(renderer, &vert, Size, Size, -Size);
        SetPosition(renderer, &vert, Size, -Size, -Size);
        SetPosition(renderer, &vert, Size, Size, Size);
        SetPosition(renderer, &vert, Size, -Size, Size);
        vert.Color = Math::Vec::Vec3(0, 1, 0);
        SetPosition(renderer, &vert, -Size, Size, -Size);
        SetPosition(renderer, &vert, -Size, -Size, -Size);
        vert.Color = Math::Vec::Vec3(0, 0, 1);
        SetPosition(renderer, &vert, -Size, Size, Size);
        SetPosition(renderer, &vert, -Size, -Size, Size);

        /// triangles
        renderer->Triangles.push_back(Vec3(4, 2, 0));
        renderer->Triangles.push_back(Vec3(2, 7, 3));
        renderer->Triangles.push_back(Vec3(6, 5, 7));
        renderer->Triangles.push_back(Vec3(1, 7, 5));
        renderer->Triangles.push_back(Vec3(0, 3, 1));
        renderer->Triangles.push_back(Vec3(4, 1, 5));
        renderer->Triangles.push_back(Vec3(4, 6, 2));
        renderer->Triangles.push_back(Vec3(2, 6, 7));
        renderer->Triangles.push_back(Vec3(6, 4, 5));
        renderer->Triangles.push_back(Vec3(1, 3, 7));
        renderer->Triangles.push_back(Vec3(0, 2, 3));
        renderer->Triangles.push_back(Vec3(4, 0, 1));
    }
    void SetICOSphere(MeshRenderer* renderer) {
        /// vertexs
        Vertex vert;
        vert.Color = Math::Vec::Vec3(1, 0, 0);
        SetPosition(renderer, &vert, Size * 0.000000, Size * -1.000000, Size * 0.000000);
        SetPosition(renderer, &vert, Size * 0.723607, Size * -0.447220, Size * 0.525725);
        SetPosition(renderer, &vert, Size * -0.276388, Size * -0.447220, Size * 0.850649);
        SetPosition(renderer, &vert, Size * -0.894426, Size * -0.447216, Size * 0.000000);
        SetPosition(renderer, &vert, Size * -0.276388, Size * -0.447220, Size * -0.850649);
        SetPosition(renderer, &vert, Size * 0.723607, Size * -0.447220, Size * -0.525725);
        SetPosition(renderer, &vert, Size * 0.276388, Size * 0.447220, Size * 0.850649);
        SetPosition(renderer, &vert, Size * -0.723607, Size * 0.447220, Size * 0.525725);
        SetPosition(renderer, &vert, Size * -0.723607, Size * 0.447220, Size * -0.525725);
        SetPosition(renderer, &vert, Size * 0.276388, Size * 0.447220, Size * -0.850649);
        SetPosition(renderer, &vert, Size * 0.894426, Size * 0.447216, Size * 0.000000);
        SetPosition(renderer, &vert, Size * 0.000000, Size * 1.000000, Size * 0.000000);
        SetPosition(renderer, &vert, Size * -0.162456, Size * -0.850654, Size * 0.499995);
        SetPosition(renderer, &vert, Size * 0.425323, Size * -0.850654, Size * 0.309011);
        SetPosition(renderer, &vert, Size * 0.262869, Size * -0.525738, Size * 0.809012);
        SetPosition(renderer, &vert, Size * 0.850648, Size * -0.525736, Size * 0.000000);
        vert.Color = Math::Vec::Vec3(0, 1, 0);
        SetPosition(renderer, &vert, Size * 0.425323, Size * -0.850654, Size * -0.309011);
        SetPosition(renderer, &vert, Size * -0.525730, Size * -0.850652, Size * 0.000000);
        SetPosition(renderer, &vert, Size * -0.688189, Size * -0.525736, Size * 0.499997);
        SetPosition(renderer, &vert, Size * -0.162456, Size * -0.850654, Size * -0.499995);
        SetPosition(renderer, &vert, Size * -0.688189, Size * -0.525736, Size * -0.499997);
        SetPosition(renderer, &vert, Size * 0.262869, Size * -0.525738, Size * -0.809012);
        SetPosition(renderer, &vert, Size * 0.951058, Size * 0.000000, Size * 0.309013);
        SetPosition(renderer, &vert, Size * 0.951058, Size * 0.000000, Size * -0.309013);
        SetPosition(renderer, &vert, Size * 0.000000, Size * 0.000000, Size * 1.000000);
        SetPosition(renderer, &vert, Size * 0.587786, Size * 0.000000, Size * 0.809017);
        SetPosition(renderer, &vert, Size * -0.951058, Size * 0.000000, Size * 0.309013);
        SetPosition(renderer, &vert, Size * -0.587786, Size * 0.000000, Size * 0.809017);
        SetPosition(renderer, &vert, Size * -0.587786, Size * 0.000000, Size * -0.809017);
        SetPosition(renderer, &vert, Size * -0.951058, Size * 0.000000, Size * -0.309013);
        vert.Color = Math::Vec::Vec3(0, 0, 1);
        SetPosition(renderer, &vert, Size * 0.587786, Size * 0.000000, Size * -0.809017);
        SetPosition(renderer, &vert, Size * 0.000000, Size * 0.000000, Size * -1.000000);
        SetPosition(renderer, &vert, Size * 0.688189, Size * 0.525736, Size * 0.499997);
        SetPosition(renderer, &vert, Size * -0.262869, Size * 0.525738, Size * 0.809012);
        SetPosition(renderer, &vert, Size * -0.850648, Size * 0.525736, Size * 0.000000);
        SetPosition(renderer, &vert, Size * -0.262869, Size * 0.525738, Size * -0.809012);
        SetPosition(renderer, &vert, Size * 0.688189, Size * 0.525736, Size * -0.499997);
        SetPosition(renderer, &vert, Size * 0.162456, Size * 0.850654, Size * 0.499995);
        SetPosition(renderer, &vert, Size * 0.525730, Size * 0.850652, Size * 0.000000);
        SetPosition(renderer, &vert, Size * -0.425323, Size * 0.850654, Size * 0.309011);
        SetPosition(renderer, &vert, Size * -0.425323, Size * 0.850654, Size * -0.309011);
        SetPosition(renderer, &vert, Size * 0.162456, Size * 0.850654, Size * -0.499995);

        /// triangles
        renderer->Triangles.push_back(Vec3(0,13,12));
        renderer->Triangles.push_back(Vec3(1,13,15));
        renderer->Triangles.push_back(Vec3(0,12,17));
        renderer->Triangles.push_back(Vec3(0,17,19));
        renderer->Triangles.push_back(Vec3(0,19,16));
        renderer->Triangles.push_back(Vec3(1,15,22));
        renderer->Triangles.push_back(Vec3(2,14,24));
        renderer->Triangles.push_back(Vec3(3,18,26));
        renderer->Triangles.push_back(Vec3(4,20,28));
        renderer->Triangles.push_back(Vec3(5,21,30));
        renderer->Triangles.push_back(Vec3(1,22,25));
        renderer->Triangles.push_back(Vec3(2,24,27));
        renderer->Triangles.push_back(Vec3(3,26,29));
        renderer->Triangles.push_back(Vec3(4,28,31));
        renderer->Triangles.push_back(Vec3(5,30,23));
        renderer->Triangles.push_back(Vec3(6,32,37));
        renderer->Triangles.push_back(Vec3(7,33,39));
        renderer->Triangles.push_back(Vec3(8,34,40));
        renderer->Triangles.push_back(Vec3(9,35,41));
        renderer->Triangles.push_back(Vec3(10,36,38));
        renderer->Triangles.push_back(Vec3(38,41,11));
        renderer->Triangles.push_back(Vec3(38,36,41));
        renderer->Triangles.push_back(Vec3(36,9,41));
        renderer->Triangles.push_back(Vec3(41,40,11));
        renderer->Triangles.push_back(Vec3(41,35,40));
        renderer->Triangles.push_back(Vec3(35,8,40));
        renderer->Triangles.push_back(Vec3(40,39,11));
        renderer->Triangles.push_back(Vec3(40,34,39));
        renderer->Triangles.push_back(Vec3(34,7,39));
        renderer->Triangles.push_back(Vec3(39,37,11));
        renderer->Triangles.push_back(Vec3(39,33,37));
        renderer->Triangles.push_back(Vec3(33,6,37));
        renderer->Triangles.push_back(Vec3(37,38,11));
        renderer->Triangles.push_back(Vec3(37,32,38));
        renderer->Triangles.push_back(Vec3(32,10,38));
        renderer->Triangles.push_back(Vec3(23,36,10));
        renderer->Triangles.push_back(Vec3(23,30,36));
        renderer->Triangles.push_back(Vec3(30,9,36));
        renderer->Triangles.push_back(Vec3(31,35,9));
        renderer->Triangles.push_back(Vec3(31,28,35));
        renderer->Triangles.push_back(Vec3(28,8,35));
        renderer->Triangles.push_back(Vec3(29,34,8));
        renderer->Triangles.push_back(Vec3(29,26,34));
        renderer->Triangles.push_back(Vec3(26,7,34));
        renderer->Triangles.push_back(Vec3(27,33,7));
        renderer->Triangles.push_back(Vec3(27,24,33));
        renderer->Triangles.push_back(Vec3(24,6,33));
        renderer->Triangles.push_back(Vec3(25,32,6));
        renderer->Triangles.push_back(Vec3(25,22,32));
        renderer->Triangles.push_back(Vec3(22,10,32));
        renderer->Triangles.push_back(Vec3(30,31,9));
        renderer->Triangles.push_back(Vec3(30,21,31));
        renderer->Triangles.push_back(Vec3(21,4,31));
        renderer->Triangles.push_back(Vec3(28,29,8));
        renderer->Triangles.push_back(Vec3(28,20,29));
        renderer->Triangles.push_back(Vec3(20,3,29));
        renderer->Triangles.push_back(Vec3(26,27,7));
        renderer->Triangles.push_back(Vec3(26,18,27));
        renderer->Triangles.push_back(Vec3(18,2,27));
        renderer->Triangles.push_back(Vec3(24,25,6));
        renderer->Triangles.push_back(Vec3(24,14,25));
        renderer->Triangles.push_back(Vec3(14,1,25));
        renderer->Triangles.push_back(Vec3(22,23,10));
        renderer->Triangles.push_back(Vec3(22,15,23));
        renderer->Triangles.push_back(Vec3(15,5,23));
        renderer->Triangles.push_back(Vec3(16,21,5));
        renderer->Triangles.push_back(Vec3(16,19,21));
        renderer->Triangles.push_back(Vec3(19,4,21));
        renderer->Triangles.push_back(Vec3(19,20,4));
        renderer->Triangles.push_back(Vec3(19,17,20));
        renderer->Triangles.push_back(Vec3(17,3,20));
        renderer->Triangles.push_back(Vec3(17,18,3));
        renderer->Triangles.push_back(Vec3(17,12,18));
        renderer->Triangles.push_back(Vec3(12,2,18));
        renderer->Triangles.push_back(Vec3(15,16,5));
        renderer->Triangles.push_back(Vec3(15,13,16));
        renderer->Triangles.push_back(Vec3(13,0,16));
        renderer->Triangles.push_back(Vec3(12,14,2));
        renderer->Triangles.push_back(Vec3(12,13,14));
        renderer->Triangles.push_back(Vec3(13,1,14));
    }
#undef SetPosition
};

#endif // __HEADER_UTILS

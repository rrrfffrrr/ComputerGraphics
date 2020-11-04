#ifndef __HEADER_VEC
#define __HEADER_VEC

/// Array of elements to represent homogeneous coordinate
namespace Math {
    namespace Vec {
        struct IVec{ const static int WIDTH = 0; };

        struct Vec3 : public IVec {
            const static int WIDTH = 3;

            union {
                struct { float x, y, z; };
                struct { float r, g, b; };
                float Value[WIDTH];
            };

            Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
        };
        struct Vec4 : public IVec {
            const static int WIDTH = 4;

            union {
                struct { float x, y, z, w; };
                struct { float r, g, b, a; };
                float Value[WIDTH];
            };

            Vec4(float x = 0, float y = 0, float z = 0, float w = 0) : x(x), y(y), z(z), w(w) {}
        };
    }
}
#endif // __HEADER_VEC

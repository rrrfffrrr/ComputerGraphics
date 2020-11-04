#ifndef __HEADER_MATRIX
#define __HEADER_MATRIX
#include "Vec.hpp"
namespace Math {
    namespace Matrix {
        using namespace Vec;

        struct IMatrix {
            const static int WIDTH = 0;
            const static int DOUBLEWIDTH = 0;
        };

        struct Matrix3x3 : public IMatrix {
            const static int WIDTH = 3;
            const static int DOUBLEWIDTH = WIDTH * WIDTH;

            union {
                struct { float
                    V00, V01, V02,
                    V10, V11, V12,
                    V20, V21, V22;
                };
                float V33[WIDTH][WIDTH];
                float Value[DOUBLEWIDTH];
            };

            Matrix3x3(float v00 = 0, float v01 = 0, float v02 = 0, float v10 = 0, float v11 = 0, float v12 = 0, float v20 = 0, float v21 = 0, float v22 = 0) : V00(v00), V01(v01), V02(v02), V10(v10), V11(v11), V12(v12), V20(v20), V21(v21), V22(v22) {}

            /// Build identity matrix
            static void BuildIdentity(Matrix3x3* output) {
                for (int i = 0; i < DOUBLEWIDTH ; ++i) { output->Value[i] = 0; }
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
                for(int i = 0; i < WIDTH; ++i) {
                    for(int j = 0; j < WIDTH; ++j) {
                        output->V33[i][j] = m1->V33[i][0] * m2->V33[0][j] + m1->V33[i][1] * m2->V33[1][j] + m1->V33[i][2] * m2->V33[2][j];
                    }
                }
            }
            static void Multiply(Vec3* v1, Matrix3x3* m1, Vec3* output) {
                for(int i = 0; i < WIDTH; ++i) {
                    output->Value[i] = v1->Value[0] * m1->V33[i][0] + v1->Value[1] * m1->V33[i][1] + v1->Value[2] * m1->V33[i][2];
                }
            }

            /// Multiply with matrix or vec3 and safe when parameter is same with output
            static void MultiplySafe(Matrix3x3* m1, Matrix3x3* m2, Matrix3x3* output) {
                Matrix3x3 temporal;
                Multiply(m1, m2, &temporal);
                memcpy(output->Value, temporal.Value, DOUBLEWIDTH * sizeof(float));
            }
            static void MultiplySafe(Vec3* v1, Matrix3x3* m1, Vec3* output) {
                Vec3 temporal;
                Multiply(v1, m1, &temporal);
                memcpy(output->Value, temporal.Value, WIDTH * sizeof(float));
            }
        };

        struct Matrix4x4 : public IMatrix {
            const static int WIDTH = 4;
            const static int DOUBLEWIDTH = WIDTH * WIDTH;

            union {
                struct { float
                    V00, V01, V02, V03,
                    V10, V11, V12, V13,
                    V20, V21, V22, V23,
                    V30, V31, V32, V33;
                };
                float V44[WIDTH][WIDTH];
                float Value[DOUBLEWIDTH];
            };

            Matrix4x4(
                      float v00 = 0, float v01 = 0, float v02 = 0, float v03 = 0,
                      float v10 = 0, float v11 = 0, float v12 = 0, float v13 = 0,
                      float v20 = 0, float v21 = 0, float v22 = 0, float v23 = 0,
                      float v30 = 0, float v31 = 0, float v32 = 0, float v33 = 0
            ) : V00(v00), V01(v01), V02(v02), V03(v03), V10(v10), V11(v11), V12(v12), V13(v13), V20(v20), V21(v21), V22(v22), V23(v23), V30(v30), V31(v31), V32(v32), V33(v33) {}

            /// Build identity matrix
            static void BuildIdentity(Matrix4x4* output) {
                for (int i = 0; i < DOUBLEWIDTH ; ++i) { output->Value[i] = 0; }
                output->V00 = output->V11 = output->V22 = output->V33 = 1;
            }
            static Matrix4x4* BuildIdentity() {
                Matrix4x4* rot = new Matrix4x4();
                BuildIdentity(rot);
                return rot;
            }

            /// Build translation matrix
            static void BuildTranslationMatrix(float tx, float ty, float tz, Matrix4x4* output) {
                *output = Matrix4x4( 1, 0, 0,  tx,
                                     0, 1, 0,  ty,
                                     0, 0, 1,  tz,
                                     0, 0, 0,   1);
            }
            static Matrix4x4* BuildTranslationMatrix(float tx, float ty, float tz) {
                Matrix4x4* rot = new Matrix4x4();
                BuildTranslationMatrix(tx, ty, tz, rot);
                return rot;
            }

            /// Build rotation matrix
            static void BuildRotationXMatrix(float degree, Matrix4x4* output) {
                float radian = M_PI * degree / 180;
                float c = cos(radian);
                float s = sin(radian);
                *output = Matrix4x4( 1, 0, 0, 0,
                                     0, c, -s, 0,
                                     0, s, c, 0,
                                     0, 0, 0, 1);
            }
            static Matrix4x4* BuildRotationYMatrix(float degree) {
                Matrix4x4* rot = new Matrix4x4();
                BuildRotationZMatrix(degree, rot);
                return rot;
            }

            static void BuildRotationYMatrix(float degree, Matrix4x4* output) {
                float radian = M_PI * degree / 180;
                float c = cos(radian);
                float s = sin(radian);
                *output = Matrix4x4( c, 0, s, 0,
                                     0, 1, 0, 0,
                                     -s, 0, c, 0,
                                     0, 0, 0, 1);
            }
            static Matrix4x4* BuildRotationZMatrix(float degree) {
                Matrix4x4* rot = new Matrix4x4();
                BuildRotationZMatrix(degree, rot);
                return rot;
            }

            static void BuildRotationZMatrix(float degree, Matrix4x4* output) {
                float radian = M_PI * degree / 180;
                float c = cos(radian);
                float s = sin(radian);
                *output = Matrix4x4( c, -s, 0, 0,
                                     s, c, 0, 0,
                                     0, 0, 1, 0,
                                     0, 0, 0, 1);
            }
            static Matrix4x4* BuildRotationXMatrix(float degree) {
                Matrix4x4* rot = new Matrix4x4();
                BuildRotationXMatrix(degree, rot);
                return rot;
            }

            /// Build scale matrix
            static void BuildScaleMatrix(float sx, float sy, float sz, Matrix4x4* output) {
                *output = Matrix4x4( sx,    0,      0,  0,
                                     0,     sy,     0,  0,
                                     0,     0,      sz, 0,
                                     0,     0,      0,  1);
            }
            static Matrix4x4* BuildScaleMatrix(float sx, float sy, float sz) {
                Matrix4x4* rot = new Matrix4x4();
                BuildScaleMatrix(sx, sy, sz, rot);
                return rot;
            }

            /// Multiply with matrix or vec3
            static void Multiply(Matrix4x4* m1, Matrix4x4* m2, Matrix4x4* output) {
                for(int i = 0; i < WIDTH; ++i) {
                    for(int j = 0; j < WIDTH; ++j) {
                        output->V44[i][j] = m1->V44[i][0] * m2->V44[0][j] + m1->V44[i][1] * m2->V44[1][j] + m1->V44[i][2] * m2->V44[2][j] + m1->V44[i][3] * m2->V44[3][j];
                    }
                }
            }
            static void Multiply(Vec4* v1, Matrix4x4* m1, Vec4* output) {
                for(int i = 0; i < WIDTH; ++i) {
                    output->Value[i] = v1->Value[0] * m1->V44[i][0] + v1->Value[1] * m1->V44[i][1] + v1->Value[2] * m1->V44[i][2] + v1->Value[3] * m1->V44[i][3];
                }
            }

            /// Multiply with matrix or vec3 and safe when parameter is same with output
            static void MultiplySafe(Matrix4x4* m1, Matrix4x4* m2, Matrix4x4* output) {
                Matrix4x4 temporal;
                Multiply(m1, m2, &temporal);
                memcpy(output->Value, temporal.Value, DOUBLEWIDTH * sizeof(float));
            }
            static void MultiplySafe(Vec4* v1, Matrix4x4* m1, Vec4* output) {
                Vec4 temporal;
                Multiply(v1, m1, &temporal);
                memcpy(output->Value, temporal.Value, WIDTH * sizeof(float));
            }
        };
    }
}
#endif // __HEADER_MATRIX
